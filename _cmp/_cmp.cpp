#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <chrono>
#include <thread>
using namespace std;
using namespace chrono;
// 4, 4(5, 6), 6, 7
// Compare: c
// Time:    t
// Judge:   j
// cmp c file1.txt file2.txt
// cmp t [language] code
// cmp t [language] code in.txt
// cmp t [language] code in.txt out.txt
// -> out.txt
// cmp j in.txt ans.txt [language] code
// C:    c  code
// C++:  cc code
// Java: j  code
// -> code.exe (code.class) out.txt
// cmp c in.txt [language1] code1 [language2] code2
// -> code1.exe out1.txt
// -> code2.exe out2.txt
bool check_empty_line(const string & s)
{
    stringstream ss(s);
    string t; ss >> t;
    return t.size() == 0 && ss.eof();
}
int compare_file(const char * file1, const char * file2)
{
    ifstream fin1(file1), fin2(file2);
    if (!fin1)
    {
        cout << "Error: Unable to open \"" << file1 << "\"" << endl;
        return -1;
    }
    if (!fin2)
    {
        cout << "Error: Unable to open \"" << file2 << "\"" << endl;
        return -1;
    }
    cout << endl << "Content Comparison: " << endl << endl;
    string s1, s2, t1, t2;
    long pos1 = 1, pos2 = 1, c = 0;
    bool content_not_identical = false, presentation_not_identical = false;
    while (1)
    {
        bool b1 = bool(getline(fin1, s1));
        while (b1 && check_empty_line(s1))
        {
            if (!getline(fin1, s1))
            {
                b1 = false;
                break;
            }
            pos1++;
        }
        if (!b1)
        {
            if (!fin2.eof())
            {
                bool b = true;
                while (getline(fin2, s2))
                {
                    if (!check_empty_line(s2))
                    {
                        if (b)
                        {
                            b = false;
                            content_not_identical = true;
                            c++;
                            cout << "The EOF of \"" << file2 << "\" is not reached." << endl << endl;
                        }
                        cout << setw(20) << pos2 << ": \t" << s2 << endl;
                    }
                    pos2++;
                }
            }
            break;
        }
        bool b2 = bool(getline(fin2, s2));
        while (b2 && check_empty_line(s2))
        {
            if (!getline(fin2, s2))
            {
                b2 = false;
                break;
            }
            pos2++;
        }
        if (!b2)
        {
            bool b = true;
            do {
                if (!check_empty_line(s1))
                {
                    if (b)
                    {
                        b = false;
                        content_not_identical = true;
                        c++;
                        cout << "The EOF of \"" << file1 << "\" is not reached." << endl << endl;
                    }
                    cout << setw(20) << pos1 << ": \t" << s1 << endl;
                }
                pos1++;
            } while (getline(fin1, s1));
            break;
        }
        stringstream ss1(s1), ss2(s2);
        bool b3 = false;
        while (1)
        {
            bool bb1 = bool(ss1 >> t1);
            bool bb2 = bool(ss2 >> t2);
            if (!bb1 || !bb2) break;
            if (t1 != t2)
            {
                b3 = true;
                content_not_identical = true;
                c++;
                break;
            }
        }
        if (!b3 && (ss1.eof() || ss1.eof()) && (!ss1.eof() || !ss2.eof()))
        {
            content_not_identical = true;
            c++; b3 = true;
        }
        else if (!b3 && s1 != s2)
        {
            presentation_not_identical = true;
            b3 = true;
        }
        if (pos1 != pos2 || b3)
        {
            presentation_not_identical = true;
            cout << setw(20) << pos1 << ": \t" << s1 << ';' << endl;
            cout << setw(20) << pos2 << ": \t" << s2 << ';' << endl << endl;
        }
        pos1++; pos2++;
    }
    cout << endl << "Result: ";
    if (content_not_identical)
        cout << "Content NOT Identical" << endl << "Difference(s): " << c << endl;
    else if (presentation_not_identical) cout << "Presentation NOT Identical" << endl;
    else cout << "Identical" << endl;
    cout << endl;
    return 0;
}
int compile_and_execute(const char * language, const char * code, const char * in, const char * out)
{
    cout << endl;
    string file_code = code;
    string file_exec = code;
    string compile_code;
    string execute_code;
    if (strcmp(out, "") != 0)
    {
        string del_out = string("if exist ") + out + " (del " + out + ")";
        system(del_out.c_str());
    }
    if (strcmp(language, "c") == 0)
    {
        file_code += ".c";
        file_exec += ".exe";
        compile_code = "gcc -o " + file_exec + " " + file_code + " -O2";
        execute_code = file_exec;
    }
    else if (strcmp(language, "cc") == 0)
    {
        file_code += ".cpp";
        file_exec += ".exe";
        compile_code = "g++ -o " + file_exec + " " + file_code + " -O2";
        execute_code = file_exec;
    }
    else if (strcmp(language, "j") == 0)
    {
        file_code += ".java";
        file_exec += ".class";
        compile_code = "javac " + file_code;
        execute_code = string("java ") + code;
    }
    else
    {
        cout << "Error: Unrecognized Language." << endl;
        return 2;
    }
    execute_code += (strcmp(in,  "") == 0 ? "" : string(" < ") + in);
    execute_code += (strcmp(out, "") == 0 ? "" : string(" > ") + out);
    string del_exec = "if exist " + file_exec + " (del " + file_exec + ")";
    system(del_exec.c_str());
    cout << "Compiling Code: " << compile_code << endl;
    if (system(compile_code.c_str()) != 0)
    {
        cout << "Error: Compilation Error." << endl;
        return -1;
    }
    this_thread::sleep_for(duration<double, milli>(100.0));
    cout << "Executing Code: " << execute_code << endl;
    auto start = steady_clock::now();
    int r = system(execute_code.c_str());
    auto end = steady_clock::now();
    duration<double, milli> d = end - start;
    cout << "Execution Time: " << d.count() << " ms." << endl;
    if (r != 0)
    {
        cout << "Error: Run-Time Error." << endl;
        return 1;
    }
    return 0;
}
int main(int argc, char * argv[])
{
    if (argc < 4 || argc > 7) return 1;
    if (strcmp(argv[1], "c") == 0)
    {
        if (argc != 4 && argc != 7) return 1;
        if (argc == 4) // cmp c file1.txt file2.txt
            compare_file(argv[2], argv[3]);
        else // cmp c in.txt [language1] code1 [language2] code2
        {
            int r1 = compile_and_execute(argv[3], argv[4], argv[2], "_out1.txt");
            int r2 = compile_and_execute(argv[5], argv[6], argv[2], "_out2.txt");
            if (!r1 && !r2)
                compare_file("_out1.txt", "_out2.txt");
        }
    }
    else if (strcmp(argv[1], "t") == 0) // cmp t [language] code in.txt out.txt
    {
        if (argc != 4 && argc != 5 && argc != 6) return 1;
        if (argc == 4) // cmp t [language] code
            compile_and_execute(argv[2], argv[3], "", "");
        else if (argc == 5) // cmp t [language] code in.txt
            compile_and_execute(argv[2], argv[3], argv[4], "");
        else // cmp t [language] code in.txt out.txt
            compile_and_execute(argv[2], argv[3], argv[4], argv[5]);
    }
    else if (strcmp(argv[1], "j") == 0) // cmp j in.txt ans.txt [language] code
    {
        if (argc != 6) return 1;
        if (compile_and_execute(argv[4], argv[5], argv[2], "_out.txt") == 0)
            compare_file(argv[3], "_out.txt");
    }
    return 0;
}
