# HOPFIELD NETWORK

In this project, a Hopfield Network is implemented that can be trained with both real and complex bits. The project will generate two executables: one for real numbers and one for complex numbers. With an interactive interface, you can upload images, choose the number of pixels of the squared grid, train with the Hebb algorithm or with the pseudoinverse, and see how the neural network can solve a corrupted pattern.


## Instructions for Compilation, Libraries, and Running

To access the project repository on GitHub, ensure Git is installed on your system, and then clone the repository with the following command:

```bash
git clone git@github.com:NicolaSebastianuttoUnibo/HopfieldNetwork.git
```

Next, inside the folder, download the necessary libraries from their GitHub repositories:
**stb**
```bash
git clone git@github.com:nothings/stb.git 
```
**imgui**
```bash
git clone git@github.com:ocornut/imgui.git  
```
```bash
git clone git@github.com:aiekick/ImGuiFileDialog.git
```
```bash
sudo apt update && sudo apt install libsdl2-dev libgl1-mesa-dev  
```
**eigen**
```bash
 git clone git@github.com:ceptontech/eigen.git
 ```


After successfully installing the required libraries, you can proceed to compile the program. For a debug build, execute the following command:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
```

For a release build, use this command:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

Once the compilation phase is complete, you can build the program that uses real numbers using:

```bash
cmake --build build  --target hopfield_real
```
For the complex one:
```bash
cmake --build build  --target hopfield_complex
```
If you want to compile the tests, run:
```bash
cmake --build build  --target all.t
```

Every time you run the program in DEBUG mode, I advise you to run this command beforehand:

```bash
export LSAN_OPTIONS=suppressions=./lsan.supp
```
To make this setting permanent, use the command:
```bash
echo 'export LSAN_OPTIONS="suppressions=/cartella/dove/hai/salvato/il/progetto/lsan.supp"' >> ~/.bashrc
```
Finally, to run the code, for real numbers:
```bash
./build/hopfield_real
```
For complex numbers:
```bash
./build/hopfield_complex
```
To run tests:
```bash
./build/all.t
```
## Commands
- Parameters can be modified using the sliders.
- You can upload any image file with <kbd>Open Images</kbd>. To select multiple images, hold <kbd>SHIFT</kbd>
- You can also generate random images with <kbd>Generate Image</kbd>. If the Noise is zero, the random image will be all black; if noise is 1, it will be a completely random image.
- If the number of patterns is small, the regrid is automatic; otherwise, remember to press  <kbd>Apply Grid</kbd>
- You can decide which training type to choose, Hebb (faster but less accurate) or Pseudoinverse (slower but more accurate), by clicking on the training type button: the text will show which type is selected.
- <kbd>Train Network</kbd> will train the network.
- <kbd>Save Trained Network</kbd> will open a window where you must choose a name for the training file.
- <kbd>Open Training</kbd> will open a previously saved training.
- <kbd>Stop!</kbd> will interrupt the training process.
- <kbd><</kbd> and <kbd>></kbd> will show the previous and next pattern.
- <kbd>Check Dimensions</kbd> will verify if you can evolve a pattern by checking dimension compatibility. If you change the grid size by mistake, the program prevents you from evolving the pattern, but this button allows you to proceed if you restore the correct size.
- <kbd>Delete</kbd> will eliminate the current pattern.
- <kbd>Corrupt</kbd> will corrupt the pattern based on the noise value: when noise=0, the corrupt image will be the same as the original; when noise=1, the image will be completely random.
- <kbd>Evolve</kbd> will evolve the corrupted pattern.
- <kbd>Stop</kbd> will stop the evolving process.
- The graph is interactive, so you can hover over points to see their values.
   
# Credits

Nicola Sebastianutto

August 2025