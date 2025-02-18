#include <bakatools.h>
#include <bakanet.h>

#define minPort 8000
#define maxPort 8000 
#define target "127.0.0.1"

using namespace Bk;
using namespace Bk::Net;

struct Binder 
{
    Scope<Socket> src;
    int port;

    Binder(Scope<Socket> src, int port)
    :src(std::move(src)), port(port) { }
    Binder(const Binder& bind) = default;
};

int main()
{
    Log::Init("Bakanet");
    auto myLocalIPs = dns_lookup("google.com", IpVersion::IPv4);
    for(auto i : myLocalIPs) BK_INFO(i);
    IpAddress ip(myLocalIPs[0]);
    HttpServer server(ip, 8080);
    server.get("/", [](HttpRequest& req)
    {
        HttpReponse res(HTTP_RES_200, req.version);
        res.body = "<h1>Bakanet</h1>";
        res.body += "<p>Working http server</p>";
        res.body += "\n<p>URL /</p>";
        return res;
    });
    server.start();
    return 0;


    /*Log::Init("Bakanet");
    IpAddress src("192.168.10.235");
    ThreadPool pool(5);
    int range = maxPort - minPort + 1;
    int ports[maxPort - minPort + 1];

    std::vector<Binder> sockets;
    sockets.reserve(range);

    for (int i = 0; i < range; i++)
    {
        int port = minPort + i;
        Binder bind = {
            CreateScope<Socket>(Socket::create(src, port, IpProtocol::TCP)),
            port
        };
        if (!(bind.src->init() && bind.src->start(10))) throw new std::exception("Error");
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
    */
}