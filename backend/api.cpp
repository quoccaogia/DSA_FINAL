#include "httplib.h"
#include "ScholarshipBackend.cpp"

int main() {
    httplib::Server server;

    server.Post("/api/student", [](const httplib::Request& req,
                                   httplib::Response& res) {

        // JSON FE gửi nằm ở đây
        cout << req.body << endl;

        res.set_content(req.body, "application/json");
    });

    server.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Methods", "GET, POST, OPTIONS"},
        {"Access-Control-Allow-Headers", "Content-Type"}
    });

    server.listen("localhost", 8080);
}