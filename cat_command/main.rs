use std::env;
use std::fs::File;
use std::path::Path;
use std::io::Read;

// `cat` command implementation

fn main() {
    let mut args = env::args().peekable();
    let _ = args.next(); // Skip program name

    // From `man cat`:
    // "With no FILE, or when FILE is -, read standard input."
    if args.peek().is_none() || args.peek().unwrap() == "-" {
        let stdin_handle = std::io::stdin();

        stdin_handle.bytes().for_each(|b| {
            print!("{}", b.unwrap() as char);
        });
    }

    args.for_each(|path_name| {
        let file = Path::new(&path_name);

        // First some checks
        if !file.exists() {
            println!("cat: {}: No such file or directory", path_name);
            return;
        }
        if !file.is_file() {
            println!("cat: {}: Is not a file", path_name);
            return;
        }

        // Open file returns `io::Result<File>` aka `Result<File, Error>`
        let open_result = File::open(&file);

        // Read file contents as bytes, because String requires valid UTF-8
        // sequence that we can't guarantee
        let mut contents: Vec<u8> = Vec::new();

        match open_result {
            Ok(mut buffer) => {
                buffer.read_to_end(&mut contents).unwrap();
            },
            Err(_) => {
                println!("cat: {}: Permission denied", path_name);
                return;
            }
        }
        println!("{}", String::from_utf8_lossy(&contents));
    });
}
