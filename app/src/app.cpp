#include <bakatools.h>
#include <bakanet.h>

#define MIN_PORT 8000
#define MAX_PORT 8000 
#define RANGE MAX_PORT - MIN_PORT + 1
#define TARGET "127.0.0.1"

using namespace Bk;
using namespace Bk::Net;

struct Binder 
{
    Scope<Socket> src = nullptr;
    Scope<Socket> dest = nullptr;
    int port = 0;

    Binder(Scope<Socket> src, int port)
    :src(std::move(src)), port(port) { }
    Binder(const Binder& bind) = default;
};

int main()
{
    //Log::Init("Bakanet");
    ////hostent* h = gethostbyname("localhost");
    ////BK_INFO(std::string(h->h_addr_list[0], h->h_length));
    //IpAddress ip("127.0.0.1");
    //HttpServer server(ip, 8080);
    //server.get("/", [](HttpRequest& req)
    //{
    //    HttpReponse res(HTTP_RES_200, req.version);
    //    res.body = "<h1>Bakanet</h1>";
    //    res.body += "<p>Working http server</p>";
    //    res.body += "\n<p>URL /</p>";
    //    return res;
    //});
    //server.start();
    //return 0;


    Log::Init("Bakanet");
    IpAddress src;
    ThreadPool pool(5);

    std::vector<Binder> sockets;
    sockets.reserve(RANGE);

    for (int i = 0; i < RANGE; i++)
    {
        int port = MIN_PORT + i;
        Binder bind = {
            CreateScope<Socket>(Socket::create(src, port, IpProtocol::TCP)),

            port
        };
        if (!(bind.src->init() && bind.src->start(10))) throw new std::exception(Tools::string_format("Couldn't bind on port %d", port));
        sockets.push_back(bind);
    }

    while (true) {
        for(const Binder& bind : sockets)
        {
            if (bind.src->hasConnection(0, 50000)) {
                Socket* socket = bind.src->ack();
                int port = bind.port;
                pool.queue([socket, port] {
                    IpAddress dest(target);
                    Type::DataStream req;
                    std::vector<char> data;
                    do
                    {
                        data = socket->obtain(1024);
                        req.append_data(data);
                    } while (data.size() >= 1024);
                    auto socketDest = Socket::create(dest, port, IpProtocol::TCP);
                    socketDest->conn();
                    socketDest->emit(req.payload);
                    Type::DataStream res;
                    do
                    {
                        data = socketDest->obtain(1024);
                        res.append_data(data);
                    } while (data.size() >= 1024);
                    socket->emit();
                });
            }
        }
    } 
    return 0;
}