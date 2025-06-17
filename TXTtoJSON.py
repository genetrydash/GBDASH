import json

def parsefurtext(txt_file, json_file):
    with open(txt_file, 'r') as input, open(json_file, 'w') as output:
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
        
        instruments = []
        
        print("Reading Instruments")

        currentintr = 0
        key = ""
        hws = []
        while True:
            
            e = input.readline()
            if e.startswith("## "):
                print(f'Instrument {e[3:].strip()}')
                
                currentintr = int(e[3:].split()[0].rstrip(":"), 16)
                instruments.append({})
                hws = []

            if e.startswith("- type: "):
                if not e[len("- type: "):].startswith("2"):
                    raise ValueError(f"What's that Non-Gameboy instrument doing Here!, Number {currentintr}")

            if e.startswith("- Game Boy parameters:"):
                key = "gb"
                instruments[currentintr][key] = {}

            if e.startswith("- Wavetable Synth parameters"):
                key = "wavesynth"
                instruments[currentintr][key] = {}

            if e.startswith("- macros:"):
                if hws:
                    instruments[currentintr][key]["hardware sequence"] = hws
                key = "macros"
                instruments[currentintr][key] = {}

            if e.startswith("  - ") and not e.startswith("  - hardware sequence:"):
                temp1 = e[4:]
                k = temp1[0:temp1.find(": ")].strip()
                v = temp1[temp1.find(": ")+2:].strip()
                if (v == "yes"):
                    v = True
                elif (v == "no"):
                    v = False
                elif (v.isnumeric()):
                    v = int(v)
                instruments[currentintr][key][k] = v

            if e.startswith("    - "):
                temp1 = e[len("    - "):].strip()
                hws.append(temp1.split(" "))

            if e.startswith("# Wavetables"):
                break

        wavetables = []
        print("Reading Wavetables")
        while True:
            e = input.readline()

            if e.startswith("- "):
                w = e[len("- 0 (32x16): "):].strip()
                wavetables.append(w.split(" "))

            if e.startswith("# Samples"):
                break


        # Skip the samples section, this format doesn't do samples
        while True:
            e = input.readline()

            if e.startswith("## 0: "):
                break
        

        songproperties = {}
        orders = []
        while True:
            e = input.readline()

            if e.startswith("- "):
                t = e[2:].strip()
                p = t.split(":")
                k = p[0].replace(" ","")
                v = p[1].replace(" ","")

                if k == "speeds":
                    songproperties[k] = v.split(" ")
                elif k == "virtualtempo":
                    songproperties[k] = v.split("/")
                else:
                    songproperties[k] = v

            if e.startswith("orders:"):
                break
            
        input.readline()
        while True:
            e = input.readline()
            if e.startswith("```") or e == "":
                break

            # Skip empty or malformed lines
            if "|" not in e:
                continue

            try:
                line = e.strip()
                _, data = line.split("|", 1)
                hex_values = data.strip().split()
                int_values = [int(x, 16) for x in hex_values]
                orders.append(int_values)
            except Exception as err:
                print(f"Error parsing order line: {e.strip()} - {err}")



        while True:
            e = input.readline()
            if e == "":
                raise ValueError("Missing '## Patterns' section")
            if e.strip() == "## Patterns":
                break

        print("Reading Patterns")
        currentorder = -1
        patterns = {1:[], 2:[], 3:[], 4:[]}
        while True:
            e = input.readline()
            


            if "|" in e and currentorder != -1:
                
                p = e.split("|")
                
                for i in range(4):
                    r = p[i+1].strip().split(" ")
                    d = {}
                    if r[0] != "...":
                        d["Note"] = r[0]

                    if r[1] != "..":
                        d["Intrument"] = r[1]

                    if r[2] != "..":
                        d["Volume"] = r[2]

                    if not all(x == "...." for x in r[3:]):
                        d["Effects"] = []
                        for e in r[3:]:
                            if e != "....":
                                d["Effects"].append(e)
                    rows[i+1].append(d)
                    

            if e.startswith("----- ORDER "):
                
                if currentorder != -1:
                    patterns[1].append({"Pattern": orders[currentorder][0], "Rows": rows[1]})
                    patterns[2].append({"Pattern": orders[currentorder][1], "Rows": rows[2]})
                    patterns[3].append({"Pattern": orders[currentorder][2], "Rows": rows[3]})
                    patterns[4].append({"Pattern": orders[currentorder][3], "Rows": rows[4]})
                rows = {1:[], 2:[], 3:[], 4:[]}
                currentorder = int(e[len("----- ORDER "):],16)

            if e == "":
                if currentorder != -1:
                    patterns[1].append({"Pattern": orders[currentorder][0], "Rows": rows[1]})
                    patterns[2].append({"Pattern": orders[currentorder][1], "Rows": rows[2]})
                    patterns[3].append({"Pattern": orders[currentorder][2], "Rows": rows[3]})
                    patterns[4].append({"Pattern": orders[currentorder][3], "Rows": rows[4]})
                break

        dd = {}
        dd["Name"] = name
        dd["Properties"] = songproperties
        dd["Instruments"] = instruments
        dd["Wavetables"] = wavetables
        dd["Orders"] = orders
        dd["Patterns"] = patterns
        output.write(json.dumps(dd))
        output.close()



parsefurtext("music/txt/fur_On_Hold.txt","fur.json")