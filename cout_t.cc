#include <cstdio>
#include <cstring>
#include <iostream>
#include <mutex>
#include <thread>

#define MAX_LOG_LEN 4096

using namespace std;

class log4c_buf : public streambuf
{

public:
    log4c_buf(ostream &stream):
        content_len(0),
        ori_ostream(stream),
        ori_buf(stream.rdbuf(this))
    {
    }

    ~log4c_buf()
    {
        ori_ostream.rdbuf(ori_buf);
    }

protected:

    virtual int_type overflow(int_type c = traits_type::eof())
    {
        //printf("overflow[%c]\n", c);
        return c;
    }

    virtual streamsize xsputn(const char *s, streamsize n)
    {
        lock_guard<mutex> lock(mtx);
        int buf_len = MAX_LOG_LEN - content_len;
        int copy_len = (buf_len > n ? n : buf_len);

        if (MAX_LOG_LEN > content_len)
        {
            memcpy(log_buf + content_len, s, copy_len);
            content_len += copy_len;
        }

        return n;
    }

    virtual int sync()
    {
        lock_guard<mutex> lock(mtx);
        log_buf[content_len] = 0;

        printf("redirected cout<%s> len[%d]\n", log_buf, content_len);
        content_len = 0;

        return 0;
    }

private:
    mutex mtx;
    char log_buf[MAX_LOG_LEN + 1];
    int content_len;
    ostream &ori_ostream;
    streambuf *ori_buf;
};

void a()
{
    cout<<__func__<<": "<<"one"<<" "<<"two"<<" "<< 1 << endl;
    cout<<__func__<<": "<<"three"<<" "<<"four"<<" "<< 1 << endl;
    cout<<__func__<<": "<<"five"<<" "<<"six"<<" "<< 1 << endl;
    cout<<__func__<<": "<<"seven"<<" "<<"eight"<<" "<< 1 << endl;
    cout<<__func__<<": "<<"nine"<<" "<<"ten"<<" "<< 1 << endl;
}

void b()
{
    cout<<__func__<<": "<<"one"<<" "<<"two"<<" "<< 1 << endl;
    cout<<__func__<<": "<<"three"<<" "<<"four"<<" "<< 1 << endl;
    cout<<__func__<<": "<<"five"<<" "<<"six"<<" "<< 1 << endl;
    cout<<__func__<<": "<<"seven"<<" "<<"eight"<<" "<< 1 << endl;
    cout<<__func__<<": "<<"nine"<<" "<<"ten"<<" "<< 1 << endl;
}

int main()
{
    log4c_buf ob(cout);

    thread t1(a);
    thread t2(b);

    t1.join();
    t2.join();

    cout << "end exec success" << endl;

    cout << "QQ zxcvbnm" << " orz" << "[" << __FUNCTION__ << "]" << 1 + 2 << " " << 4 << endl;

    cout << "QQ one" <<" " "two" << endl;
    cout << "QQ one" <<" " "two" << 1 << endl;
    cout << "QQ one" <<" " "two" << 2 << endl;
    cout << "QQ one" <<" " "two" << 3 << endl;

    return 0;
}
