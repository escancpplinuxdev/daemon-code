
#include <iostream>
#include <fstream>

int ostreamfileopen()
{
    // 1. Open the file in standard TEXT mode (Notice: std::ios::binary is REMOVED)
    std::ofstream fp("/tmp/data.txt", std::ios::out);
    if (!fp)
    {   
        std::cerr << "Error opening file!\n";
        return 1;
    }

    int data[5] = {1, 2, 3, 4, 5};

    // 2. Loop through the array and write each integer as readable text characters
    for (int i = 0; i < 5; ++i)
    {
        fp << data[i];
        
        // Add a space or a newline between numbers so they don't bunch up as "12345"
        if (i < 4) {
            fp << " "; 
        }
    }
    
    fp << "\n"; // Clean newline at the end

    // 3. Close the file stream
    fp.close();
    
    std::cout << "Text data written successfully via C++.\n";
    return 0;
}



int ostreamfileopen_1()
{
    // 1. Open the file in binary write mode
    std::ofstream fp("/tmp/data.bin", std::ios::out | std::ios::binary);
    if (!fp) 
    {
        std::cerr << "Error opening file!\n";
        return 1;
    }

    int data[5] = {1, 2, 3, 4, 5};

    // 2. Write raw bytes directly to disk
    // (reinterpret_cast converts the int* pointer to a char* pointer)
    fp.write(reinterpret_cast<const char*>(data), sizeof(data));

    // 3. Close the file stream
    fp.close();

    std::cout << "Binary data written successfully via C++.\n";
    return 0;
}


#include <iostream>
#include <fstream>

int fileopen()
{
    FILE* fp = fopen("/tmp/data.bin", "w+");
    if (!fp) return 1;
    
    int data[5] = {1, 2, 3, 4, 5};
    fwrite(data, sizeof(int), 5, fp);
    
    fclose(fp);
    return 0;
}

int main()
{

    ostreamfileopen();
	
//fileopen();

    // popen requires FILE*
//    FILE* fp = popen("ls -l", "r");
    FILE* fp = popen("cat /tmp/data.bin", "r");
    if (!fp) return 1;
    
    char buf[256];
    while (fgets(buf, sizeof(buf), fp))
        printf("%s", buf);
    
    pclose(fp);
    return 0;
}
