#include <gflags/gflags.h>
#include <butil/logging.h>
#include <brpc/server.h>
#include "mailsvr.pb.h"

//执行时还要加lib path: export LD_LIBRARY_PATH="/home/wcc/workspace/copy_universe_brpc/dep/brpc/output/lib/"

DEFINE_bool(send_attachment, true, "Carry attachment along with response");
DEFINE_int32(port, 8003, "TCP Port of this server");
DEFINE_int32(idle_timeout_s, -1, "Connection will be closed if there is no "
	"read/write operations during the last `idle_timeout_s'");
DEFINE_int32(logoff_ms, 2000, "Maximum duration of server's LOGOFF state "
	"(waiting for client to close connection before server stops)");

// Your implementation of mailsvr::MailsvrService
class MailsvrServiceImpl : public mailsvr::MailsvrService {
public:
	MailsvrServiceImpl() {};
	virtual ~MailsvrServiceImpl() {};
	virtual void GetMailList(google::protobuf::RpcController* cntl_base,
		const mailsvr::GetMailListRequest* request,
		mailsvr::GetMailListResponse* response,
		google::protobuf::Closure* done) {
		brpc::ClosureGuard done_guard(done);

		LOG(INFO) << ": " << request->message() << noflush;

		// Fill response.
		response->set_message(request->message());
	}
};

int main(int argc, char* argv[]) {
	// Parse gflags. We recommend you to use gflags as well.
	GFLAGS_NS::ParseCommandLineFlags(&argc, &argv, true);

	// Generally you only need one Server.
	brpc::Server server;

	// Instance of your service.
	MailsvrServiceImpl mailsvr_service_impl;

	// Add the service into server. Notice the second parameter, because the
	// service is put on stack, we don't want server to delete it, otherwise
	// use brpc::SERVER_OWNS_SERVICE.
	if (server.AddService(&mailsvr_service_impl,
		brpc::SERVER_DOESNT_OWN_SERVICE) != 0) {
		LOG(ERROR) << "Fail to add service";
		return -1;
	}

	// Start the server.
	brpc::ServerOptions options;
	options.idle_timeout_sec = FLAGS_idle_timeout_s;
	if (server.Start(FLAGS_port, &options) != 0) {
		LOG(ERROR) << "Fail to start MailServer";
		return -1;
	}

	// Wait until Ctrl-C is pressed, then Stop() and Join() the server.
	server.RunUntilAskedToQuit();
	return 0;
}