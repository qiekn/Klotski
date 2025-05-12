# Quickstart

```bash
git clone https://github.com/qiekn/raylib-quickstart.git
cmake -B build && make -j$(nproc) -C build
./build/game
```

`compile_commands.json` for neovim lsp config:

```bash
cd path/to/your/project
ln -s build/compile_commands.json ./compile_commands.json
```
