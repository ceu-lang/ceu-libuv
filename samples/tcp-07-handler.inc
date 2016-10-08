#ifndef _UV_TCP_SERVER_HANDLER_CEU
#define _UV_TCP_SERVER_HANDLER_CEU

code/await UV_TCP_Server_Handler (var& _uv_tcp_t tcp) => int
do
    loop do
        vector[] byte string;
        await UV_TCP_ReadLine(&tcp, &string);
        string = string..[{'\0'}];
        _printf("[server %p] %s", &&tcp, &&string[0]);
        await UV_TCP_Write(&tcp, &string);
    end
end

#endif
