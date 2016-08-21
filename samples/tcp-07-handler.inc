#ifndef _UV_TCP_SERVER_HANDLER_CEU
#define _UV_TCP_SERVER_HANDLER_CEU

code/await UV_TCP_Server_Handler (var& _uv_stream_t stream) => int
do
    loop do
        vector[] byte string;
        await UV_Stream_ReadLine(&stream, &string);
        string = string..[{'\0'}];
        _printf("[server %p] %s", &&stream, &&string[0]);
        await UV_Stream_Write(&stream, &string);
    end
end

#endif
