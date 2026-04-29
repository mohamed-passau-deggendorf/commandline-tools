use std::fs::File;
use std::io::{BufRead, BufReader};
use std::env;

fn main() {
    let args: Vec<String> = env::args().collect();
    
    if args.len() != 3 {
        eprintln!("Usage: {} <filename> <search_string>", args[0]);
        std::process::exit(1);
    }
    
	
    
    let file = File::open(&args[1]).expect("Failed to open file");
    let reader = BufReader::new(file);
    
    for (line_num, line) in reader.lines().enumerate() {
        let line = line.expect("Failed to read line");
        if line.contains(&args[2]) {
            println!("{}: {}", line_num + 1, line);
        }
    }
}
