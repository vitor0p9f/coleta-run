
# ♻️ Coleta Run

This game was developed as part of the **Computer Graphics** course in the **Computer Engineering program at UNIVASF**, and is based on the concept of **selective waste collection**.

---

## 📝 Summary

🎮 **Coleta Run** is a **2D competitive multiplayer game** where two players compete to **collect and correctly dispose of garbage bags** within a time limit.

🗑️ Different categories of garbage appear randomly across the map.  
🚶‍♂️ Each player must **race to collect the bags** and **deliver them to the correct trash bin** to earn points.

🏆 At the end of the game, the player with the **highest score wins**.

---

## 🎥 Showcase

[![Game Demo](https://github.com/user-attachments/assets/ac5ff744-3156-4eaa-adbd-daf8099ebe7f)](https://github.com/user-attachments/assets/ac5ff744-3156-4eaa-adbd-daf8099ebe7f)

---

## 🛠️ Build Instructions

This project uses a simple **Makefile** for building the executable.  
It supports both **Linux 🐧** and **Windows 🪟 (MinGW or similar environments)**.

---

### ✅ Requirements

| 🖥️ Platform | 📦 Requirements |
|---------|-------------|
| **Linux** | `g++`, OpenGL development libraries (`libgl`, `libglu`, `freeglut`), and `make` |
| **Windows** | MinGW or compatible GCC toolchain, FreeGLUT library (`freeglut.lib`, `opengl32.lib`, `glu32.lib`), and `make` (e.g., via MSYS2, Git Bash, or MinGW Makefiles) |

---

### 🚀 Build Steps

#### 🐧 Linux:

```bash
make
```

✅ This will produce an executable file:

```bash
./executable
```

---

#### 🪟 Windows (MinGW + MSYS2 or similar):

```bash
make
```

✅ This will produce:

```bash
executable.exe
```

📂 Make sure you have the FreeGLUT `.lib` files available at:

```bash
./lib/x64/
```

---

### 🧹 Cleaning Build Artifacts

To remove compiled object files and the executable:

```bash
make clean
```

✅ Works on both **Linux** and **Windows**.

---

## 💡 Notes

- ✅ The **Makefile automatically detects the OS** and adjusts compiler flags and linker options.
- 🪟 On **Windows**, it expects FreeGLUT and OpenGL libraries in:

```bash
./lib/x64/
```

- 🐧 On **Linux**, it links against **system-installed OpenGL and GLUT development libraries**.
