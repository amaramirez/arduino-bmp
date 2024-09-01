from os import listdir, system, path

global dir_list, current_page, total_pages, items_per_page, command, current_files, quit_flag

# initialize vars for directory
dir_list = []
current_page = 0
current_files = []
total_pages = 0
items_per_page = 5
command = None
quit_flag = False

def do_clear():
    system("clear") # Uncomment for Bash
    #system("CLS") # Uncomment for Windows

def read_current_directory():
    global dir_list, current_page, total_pages, items_per_page, current_files

    print("Reading directory...")

    tmp_dir_list = listdir("./")
    for file in tmp_dir_list:
        if file.endswith(".bmp"):
            dir_list.append(file)
    total_pages = int(len(dir_list) / items_per_page)
    
    print("done.")
    print("Total pages:\t" + str(total_pages))

def print_files():
    global total_pages, current_page, items_per_page, dir_list, current_files

    file_list = ""
    for i in range(len(current_files)):
        file_name = current_files[i]
        file_list += str(i+1) + ") " + file_name
        if (i < len(current_files) - 1):
            file_list += "\n\t"
    print("Files:")
    print("\t" + file_list)
    print("Page " + str(current_page+1) + "/" + str(total_pages+1))
    print("B - Back / N - Next, type number or filename, or type @A for all files, Q - Quit")

def choose_file():
    global command, current_page, total_pages, current_files, quit_flag, dir_list

    selection = None
    error = None
    while (command == None):
        do_clear()
        page_index = current_page * items_per_page
        current_files = dir_list[page_index:page_index+items_per_page]
        print_files()

        selectable = ["B","N", "@A"]
        for i in range(len(current_files)):
            selectable.append(str(i+1))
        
        if (error != None):
            print(error)
        error = None
        selection = input("Command: ")
        if (selection.upper() == "Q"):
            quit_flag = True
            break
        if (selection.upper() not in selectable and selection not in dir_list):
            error = "Select a valid option"
            continue
        if (selection.upper() == "B" and current_page > 0):
            current_page -= 1
            continue
        if (selection.upper() == "N" and current_page < total_pages):
            current_page += 1
            continue
        if (selection.isdigit()):
            command = current_files[int(selection)-1]
        else:
            command = selection

def convert(bmp_path):
    bmp = open(bmp_path, "rb")
    bmp_name = bmp_path[:-4]

    bmp.seek(0)
    signature = bmp.read(2)
    if (signature != b'BM'):
        print(bmp_path + " - Not a valid BMP file")
        return
    print("Name:\t\t" + bmp_name)

    bmp.seek(0x0012)
    width = int.from_bytes(bmp.read(4), "little")

    bmp.seek(0x0016)
    height = int.from_bytes(bmp.read(4), "little")

    bmp_name_height = bmp_name.upper() + "_HEIGHT"
    bmp_name_width = bmp_name.upper() + "_WIDTH "
    
    print("Width:\t\t"+ str(width)+"\nHeight:\t\t"+str(height))

    bmp.seek(0x000A)
    data_offset = int.from_bytes(bmp.read(4), "little")
    print("Data Offset:\t"+str(data_offset))

    bmp.seek(data_offset)
    data = list(bmp.read())
    

    data_length = len(data)
    line_buffer = int(data_length/height)
    
    skip_bytes = (4-(4*(width/32)))%4
    if (width % 32 == 0):
        skip_bytes = 0

    print("Line Buffer:\t" + str(line_buffer))
    print("Skip Bytes:\t" + str(skip_bytes) + "\n")

    cpp_str = ""
    cpp_str += "#define " + bmp_name_height + " " + str(height) + "\n"
    cpp_str += "#define " + bmp_name_width + " " + str(width) + "\n"
    cpp_str += "static const unsigned char PROGMEM " + bmp_name + "[] = {"

    for i in range(height):
        cpp_str += "\n\t"
        row_index = (data_length) - line_buffer * (i + 1)
        realistic = line_buffer - skip_bytes

        for j in range(line_buffer):
            if (j > realistic - 1):
                continue
            data_byte = data[row_index + j]
            cpp_str += "B"+format(data_byte, '08b')
            if (j < realistic - 1):
                cpp_str += ","
        if (i < height -1):
            cpp_str += ","
        
    cpp_str += "\n};\n\n"
    
    cpp_str += "//Copy this to after initialization\n"
    cpp_str += "display.clearDisplay();\n"
    cpp_str += "display.drawBitmap(0, 0,  " + bmp_name + ", " + bmp_name_width + ", " + bmp_name_height + ", 1);\n"
    cpp_str += "display.display();\n"
    
    bmp.close()

    cpp_file = open(bmp_name + ".txt", "w")
    cpp_file.write(cpp_str)
    cpp_file.close()

def main():
    global command
    read_current_directory()
    while (True):
        choose_file()
        if (quit_flag == True):
            break
        do_clear()
        print("Converting...")
        if (command == "@A"):
            for file_name in dir_list:
                convert(file_name)
        else:
            convert(command)

        input("Done, press any key to continue")
        command = None

    print("Thank you, bye! :)")
    

main()
