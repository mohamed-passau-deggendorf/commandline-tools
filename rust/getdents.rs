use std::ffi::CString;
use std::ptr;


type c_int = i32;
type c_uint = u32;
type c_void = ();


extern "C" {
    fn open_directory(pathname: *const i8) -> c_int;
    fn get_directory_entries(fd: c_uint, buffer: *mut c_void, size: c_uint) -> c_int;
    fn close_directory(fd: c_int);
}

fn main() {
    let args: Vec<String> = std::env::args().collect();
    if args.len() < 2 {
        eprintln!("Usage: {} <directory>", args[0]);
        std::process::exit(1);
    }

    let path = CString::new(args[1].as_str()).unwrap();
    
    unsafe {
        let fd = open_directory(path.as_ptr());
        
        if fd < 0 {
            eprintln!("Failed to open directory");
            std::process::exit(1);
        }

        let mut buffer = vec![0u8; 8192];
        
        loop {
            let nread = get_directory_entries(fd as c_uint, buffer.as_mut_ptr() as *mut c_void, 8192);
            
            if nread <= 0 {
                break;
            }
            
            let mut bpos = 0;
            while bpos < nread as usize {
                let d = buffer.as_ptr().add(bpos);               
                let reclen = *(d.add(16) as *const u16);                
                let name_ptr = d.add(19);               
                let mut name = String::new();
                let mut k = 0;
                loop {
                    let c = *name_ptr.add(k);
                    if c == 0 {
                        break;
                    }
                    name.push(c as char);
                    k += 1;
                }
                
                println!(" {}", name);
                bpos += reclen as usize;
            }
        }
        
        close_directory(fd);
    }
}
