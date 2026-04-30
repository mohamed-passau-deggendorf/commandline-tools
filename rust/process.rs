use std::ffi::CString;
use std::os::unix::io::RawFd;
use std::ptr;
use std::str;


type c_int = i32;
type c_uint = u32;
type c_void = ();
type c_long = i64;
type size_t = usize;
type ssize_t = isize;


extern "C" {

    fn open_directory(pathname: *const i8) -> c_int;
    fn get_directory_entries(fd: c_uint, buffer: *mut c_void, size: c_uint) -> c_int;
    fn close_directory(fd: c_int);
    fn openat_wrapper(dirfd: c_int, pathname: *const i8, flags: c_int) -> c_int;
    fn readlinkat_wrapper(dirfd: c_int, pathname: *const i8, buf: *mut i8, bufsiz: size_t) -> ssize_t;
    fn getpid_wrapper() -> c_int;
}

const BUF_SIZE: usize = 8192;
const O_RDONLY: c_int = 0;
const O_DIRECTORY: c_int = 0x10000;

fn read_link(fd: c_int, path: &str) -> String {
    let path_cstr = CString::new(path).unwrap();
    let mut buffer = vec![0u8; 2048];
    
    unsafe {
        let len = readlinkat_wrapper(
            fd,
            path_cstr.as_ptr(),
            buffer.as_mut_ptr() as *mut i8,
            buffer.len()
        );
        
        if len > 0 {
            buffer.truncate(len as usize);
            String::from_utf8_lossy(&buffer).to_string()
        } else {
            String::new()
        }
    }
}


fn main() {
    let args: Vec<String> = std::env::args().collect();
    let search_pattern = if args.len() >= 2 { &args[1] } else { "" };
    
    unsafe {
        let fd = open_directory("/proc\0".as_ptr() as *const i8);
        if fd < 0 {
            eprintln!("Failed to open /proc");
            std::process::exit(1);
        }
        
        let mut buffer = vec![0u8; BUF_SIZE];
        
        loop {
            let nread = get_directory_entries(
                fd as c_uint,
                buffer.as_mut_ptr() as *mut c_void,
                BUF_SIZE as c_uint
            );
            
            if nread <= 0 {
                break;
            }
            
            let mut bpos = 0;
            while bpos < nread as usize {
                let d = buffer.as_ptr().add(bpos);
                let reclen = *(d.add(16) as *const u16);
                let d_type = *(d.add(18) as *const u8);
                let name_ptr = d.add(19);
                
                let mut name_bytes = Vec::new();
                let mut k = 0;
                loop {
                    let c = *name_ptr.add(k);
                    if c == 0 { break; }
                    name_bytes.push(c);
                    k += 1;
                }
                
                let name = String::from_utf8_lossy(&name_bytes);
                
                let mut pid = 0;
                let mut is_pid = true;
                for &c in &name_bytes {
                    if c >= b'0' && c <= b'9' {
                        pid = pid * 10 + (c - b'0') as i32;
                    } else {
                        is_pid = false;
                        break;
                    }
                }
                
                if d_type == 4  && name != "." && name != ".." && is_pid && pid > 0  && pid != getpid_wrapper()
                {
                    let proc_path = CString::new(name.to_string()).unwrap();
                    let pfd = openat_wrapper(fd, proc_path.as_ptr(), O_RDONLY | O_DIRECTORY);
                    
                    if pfd >= 0 {
                        let exe_path = read_link(pfd, "exe");
			    let path = format!("/proc/{}/cmdline", pid);
			 let cmdline =    match std::fs::read_to_string(&path) {
				Ok(content) => content.replace('\0', " ").trim().to_string(),
				Err(_) => String::new(),
			    };
						
                      

			if search_pattern.is_empty()
                            || cmdline.contains(search_pattern) || exe_path.contains(search_pattern) || name.contains(search_pattern)
			{
                            println!("{} {} {}", cmdline, pid, exe_path);
                            
                            let fd_path = CString::new("fd").unwrap();
                            let fdfd = openat_wrapper(pfd, fd_path.as_ptr(), O_RDONLY | O_DIRECTORY);
                            
                            if fdfd >= 0 {
                                let mut fd_buffer = vec![0u8; BUF_SIZE];
                                
                                loop {
                                    let fd_nread = get_directory_entries(
                                        fdfd as c_uint,
                                        fd_buffer.as_mut_ptr() as *mut c_void,
                                        BUF_SIZE as c_uint
                                    );
                                    
                                    if fd_nread <= 0 { break; }
                                    
                                    let mut fd_bpos = 0;
                                    while fd_bpos < fd_nread as usize {
                                        let fd_d = fd_buffer.as_ptr().add(fd_bpos);
                                        let fd_reclen = *(fd_d.add(16) as *const u16);
                                        let fd_d_type = *(fd_d.add(18) as *const u8);
                                        let fd_name_ptr = fd_d.add(19);
                                        

                                        let mut fd_name_bytes = Vec::new();
                                        let mut k2 = 0;
                                        loop {
                                            let c = *fd_name_ptr.add(k2);
                                            if c == 0 { break; }
                                            fd_name_bytes.push(c);
                                            k2 += 1;
                                        }
                                        let fd_name = String::from_utf8_lossy(&fd_name_bytes);
                                        

                                        if fd_d_type == 10 {
                                            let link_path = read_link(fdfd, &fd_name);
                                            println!("\t{}", link_path);
                                        }
                                        
                                        fd_bpos += fd_reclen as usize;
                                    }
                                }
                                
                                close_directory(fdfd);
                            }
                        }
                        
                        close_directory(pfd);
                    }
                }
                
                bpos += reclen as usize;
            }
        }
        
        close_directory(fd);
    }
}
