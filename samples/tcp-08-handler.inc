#ifndef _UV_TCP_SERVER_HANDLER_CEU
#define _UV_TCP_SERVER_HANDLER_CEU

class UV_TCP_Server_Handler with
    var _uv_tcp_t& tcp;
    function (_uv_tcp_t& tcp)=>UV_TCP_Server_Handler build;
do
    function (_uv_tcp_t& tcp)=>UV_TCP_Server_Handler build do
        this.tcp = &tcp;
    end

    loop do
        var char[] string;
        do UV_Stream_ReadLine.build(&_UV_STREAM_ALIAS(tcp), &string);
        _printf("[server %p] %s", &&tcp, (_char&&)&&string);
        do UV_Stream_Write.build(&_UV_STREAM_ALIAS(tcp), &string);
    end
end

#endif
