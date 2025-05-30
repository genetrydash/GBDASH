def parsefurtext(txt_file, c_file):
    song_name = "Unknown"  # default if not found
    
    with open(txt_file, 'r', encoding='utf-8') as f_in:
        lines = f_in.readlines()
        
        # Look for the song name line under '# Song Information'
        inside_song_info = False
        for line in lines:
            stripped = line.strip()
            if stripped == "# Song Information":
                inside_song_info = True
                continue
            if inside_song_info:
                if stripped.startswith("- name:"):
                    # Extract song name after "- name:"
                    song_name = stripped[len("- name: "):].strip()
                    break
                # If another header or blank line is encountered, stop searching
                if stripped.startswith("#") or stripped == "":
                    break
    
    with open(c_file, 'w', encoding='utf-8') as f_out:
        f_out.write("/*\n")
        f_out.write(f"Converted from {txt_file}\n")
        f_out.write(f"Name: {song_name}\n")
        f_out.write("*/\n\n")
        
        # You can add more processing and writing here later
