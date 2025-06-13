def parsefurtext(txt_file, c_file):
    with open(txt_file, 'r') as input, open(c_file, 'w') as output:
        e = ""

        # Extract Name
        while not e.startswith("- name: "):
            e = input.readline()
            if e == "":
                raise ValueError("Invalid File: Missing '- name:' line")
        name = e[len("- name: "):].strip()

        print(f'Name: {name}')

        # Validate The File: Look for Sound Chips section
        while not e.startswith("# Sound Chips"):
            e = input.readline()
            if e == "":
                raise ValueError("Invalid File: Missing '# Sound Chips' section")

        # Check for Game Boy and other chips
        seengb = False
        others = False
        while True:
            e = input.readline()
            if e == "":
                raise ValueError("Invalid File: Unexpected EOF before '# Instruments'")

            if e.startswith("# Instruments"):
                break

            if e.startswith("- "):
                chip_name = e[2:].strip()
                if chip_name.startswith("Game Boy"):
                    if not seengb:
                        seengb = True
                    else:
                        others = True  # Multiple Game Boy chips?
                else:
                    others = True

        # Final validation
        if not seengb:
            raise ValueError("The module should contain a Game Boy chip")
        if others:
            raise ValueError("The module should only contain one Game Boy chip and no others")

        print("Check passed")
        output.write(f"Name: {name}\nValidated: OK\n")
        
        instruments = []
        
        print("Reading Instruments")

        currentintr = 0
        while True:
            
            e = input.readline()
            if e.startswith("## "):
                print(f'Instrument {e[3:].strip()}')
                currentintr = int(e[3:].split()[0].rstrip(":"), 16)

            if e.startswith("# Wavetables"):
                break


        



parsefurtext("music/txt/fur_dsads.txt","o.c")