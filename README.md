# OS website

A website based on a 32-bit OS, based on os-tutorial from: https://github.com/cfenollosa/os-tutorial.

The project is for a portfolio site of me and serves no commercial purpose.

## Build/Run instruction (macOS)
1. Install assembler and emulator 

    `brew install qemu nasm`
2. Install cross-compiler

    Follow instructions at: https://github.com/nativeos/homebrew-i386-elf-toolchain

3. Build the project

    `cd ./os/src`

    `make run`

4. Run the project with an web-based emulator (based on v86.js)

    `python3 -m http.server 1234`

## TODOs
- [x] Bootloader
- [x] 32-bit kernel
- [x] Screen and keyboard driver
- [ ] Serial driver
- [ ] Portfolio program
- [ ] TBD
