#include <iostream>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>
#include <pwd.h>


static bool is_running = true;
using namespace std;

// 分隔符分割string型字符串，存入vector容器， deprecated
void split(const string& s, vector<string>& tokens, const string& delimiters=" "){
    string::size_type  last_pos = s.find_first_not_of(delimiters, 0);
    string::size_type  pos = s.find_first_of(delimiters, last_pos);
    while (string::npos != pos || string::npos != last_pos){
        tokens.push_back(s.substr(last_pos, pos-last_pos));
        last_pos = s.find_first_not_of(delimiters, pos);
        pos = s.find_first_of(delimiters, last_pos);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    while (is_running) {
        char command_buf[1024];      // 输入命令行buffer
        char *command = command_buf; // 命令字符串
        char *command_arg[64];       // 命令参数列表（包含命令）
        char *p = command_buf;       // 临时指针

        // 输出提示符
        struct passwd *current_user;
        char host_name[256];
        char *current_path;
        current_user = getpwuid(getuid());
        gethostname(host_name, 256);
        current_path = get_current_dir_name();
        cout << "\n# " << current_user->pw_name;  // 用户名
        cout << " @ " << host_name;               // 主机名
        cout << " in " << current_path;           // 当前目录
        cout << "\n$ㅤ" << flush;

        // 输入命令
        char c;
        while ((c = cin.get()) != '\n'){
            *p++ = c;
        }
        *p = '\0';
        cin.clear();
        cin.sync();

        // 解析命令
        if (*command == '\0') continue;
        while (*command == ' ')
            command += 1;
        p = command;
        command_arg[0] = command;
        int i = 1;
        while (*p != '\0'){
            if (*p == ' '){
                *p = '\0';
                command_arg[i++] = ++p;
            } else
                ++p;
        }
        command_arg[i] = nullptr;

        // 执行命令
        if (strcmp(command, "exit") == 0)
            is_running = false;
        else if (strcmp(command, "cd") == 0)
            cout << "my_sh: not support yet." << endl;
        else{
            pid_t pid = fork();
            if (pid == 0){
                int ret = execvp(command, command_arg);
                if (ret == -1) cout << "my_sh: command not found: " << command << endl;
                return ret;
            }else{
                pid_t ret = waitpid(pid, nullptr, 0);
            }
        }
    }
    return 0;
}
