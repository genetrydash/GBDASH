import os
import sys
import subprocess
import platform


def clear_terminal():
    if platform.system() == "Windows":
        os.system("cls")
    else:
        os.system("clear")

input_dir = "music/modules"
output_dir = "music/txt"
burner = subprocess.DEVNULL

# Clear output_dir before running
if os.path.isdir(output_dir):
    for f in os.listdir(output_dir):
        file_path = os.path.join(output_dir, f)
        if os.path.isfile(file_path):
            os.remove(file_path)
else:
    os.makedirs(output_dir)

script_dir = os.path.dirname(os.path.abspath(__file__))

# Determine OS
is_windows = platform.system() == "Windows"

# Set executable paths based on OS
if is_windows:
    furnace_path = os.path.join("processers", "FUR", "furnace.exe")
    lsdj2txt_path = os.path.join("processers", "LSDJ", "lsdj2txt.exe")
else:
    # Use Linux/Mac native binaries or scripts - you need to provide these or install them
    furnace_path = os.path.join(script_dir, "processers", "FUR", "furnace")       # No .exe
    lsdj2txt_path = os.path.join(script_dir, "processers", "LSDJ", "lsdj2txt")    # No .exe

# Check if executables exist and are executable
for exe_path in [furnace_path, lsdj2txt_path]:
    if not os.path.isfile(exe_path) or not os.access(exe_path, os.X_OK):
        print(f"Error: Executable '{exe_path}' not found or not executable on this OS.")
        sys.exit(1)

def furnace(input_file, output_file):
    subprocess.run(
        [furnace_path, "-txtout", output_file, "-noreport", input_file, "-loglevel", "error"],
        check=True,
        stdout=burner,
        stderr=burner
    )

def lsdj(input_file, output_file):
    with open(output_file, "w", encoding="utf-8") as out_file:
        subprocess.run(
            [lsdj2txt_path, input_file],
            check=True,
            stdout=out_file,
            stderr=burner
        )

if not os.path.isdir(input_dir):
    print(f"Input directory '{input_dir}' does not exist.")
    sys.exit(1)

for filename in os.listdir(input_dir):
    filepath = os.path.join(input_dir, filename)
    if not os.path.isfile(filepath):
        continue
    base, ext = os.path.splitext(filename)
    ext = ext.lower().lstrip('.')

    if ext == "fur":
        prefix = "fur"
    elif ext in ["srm", "sav"]:
        prefix = "lsdj"
    else:
        print(f"Warning: Unrecognized file extension '{ext}' for file '{filename}'")
        continue

    output_filename = f"{prefix}_{base}.txt"
    output_path = os.path.join(output_dir, output_filename)

    try:
        if prefix == "fur":
            clear_terminal()
            print(f'Processing Furnace Module {base}')
            furnace(filepath, output_path)
        else:
            clear_terminal()
            print(f'Processing LSDJ Module {base}')
            lsdj(filepath, output_path)
    except subprocess.CalledProcessError as e:
        print(f"Error processing '{filename}': {e}")

clear_terminal()
print("Done Processing")