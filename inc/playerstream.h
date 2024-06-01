#ifndef PLAYERSTREAM_H
#define PLAYERSTREAM_H()

#include <functional>
#include <iostream>
#include <memory>
#include <streambuf>
#include <system_error>

class playerbuf : public std::streambuf {
   public:
    playerbuf(int input_fd, int output_fd);
    ~playerbuf();
    static constexpr int BUF_SIZE = 1024;

    void set_timeout_ms(int timeout_ms);

    using error_fun_t =
        std::function<void(const playerbuf& sender, int errnum)>;
    inline void on_error_call(error_fun_t error_fun);
    inline void on_error_throw();
    inline void on_error_no_op();
    inline int get_last_error() const;
    std::string get_last_strerror() const;

    playerbuf(const playerbuf&) = delete;
    playerbuf& operator=(const playerbuf&) = delete;

   protected:
    int underflow() override;
    int overflow(int c) override;
    int sync() override;

   private:
    static void throw_last_error(const playerbuf& sender, int errnum);
    void call_on_error() const;
    int input_fd_;
    int output_fd_;
    char* readbuf_;
    char* writebuf_;
    std::unique_ptr<timeval> timeout_;
    error_fun_t on_error_;
    int last_error_;
};

class playerstream_base {
   public:
    inline void set_timeout_ms(int timeout_ms);

    inline void on_error_call(playerbuf::error_fun_t error_fun);

    inline void on_error_throw();

    inline void on_error_no_op();

    inline int get_last_error() const;

    inline std::string get_last_strerror() const;

    static void ignore_sigpipe();

   protected:
    playerbuf pbuf_;
    playerstream_base(int input_fd, int output_fd)
        : pbuf_(input_fd, output_fd) {}
};

class iplayerstream : public virtual playerstream_base, public std::istream {
   public:
    iplayerstream(int input_fd)
        : playerstream_base(input_fd, -1),
          std::ios(&pbuf_),
          std::istream(&pbuf_) {}
};

class oplayerstream : public virtual playerstream_base, public std::ostream {
   public:
    oplayerstream(int output_fd)
        : playerstream_base(-1, output_fd),
          std::ios(&pbuf_),
          std::ostream(&pbuf_) {}
};

class playerstream : public virtual playerstream_base,
                     public std::istream,
                     public std::ostream {
   public:
    playerstream(int input_fd, int output_fd)
        : playerstream_base(input_fd, output_fd),
          std::ios(&pbuf_),
          std::istream(&pbuf_),
          std::ostream(&pbuf_) {}
};

class playerbuf_error : public std::system_error {
   public:
    playerbuf_error(std::string msg, std::error_code err_code);
    const char* what() const noexcept override;
    std::error_code get_error_code() const;

   private:
    const std::string what_;
    const std::error_code err_code_;
};

#include "playerstream_inlines.h"

#endif  // !PLAYERSTREAM_H
