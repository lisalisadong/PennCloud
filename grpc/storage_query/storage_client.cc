#include <iostream>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>
#include "storage_query.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using storagequery::StorageQuery;
using storagequery::GetRequest;
using storagequery::GetResponse;
using storagequery::PutRequest;
using storagequery::PutResponse;
using storagequery::CPutRequest;
using storagequery::CPutResponse;
using storagequery::DeleteRequest;
using storagequery::DeleteResponse;

class StorageClient {
public:
	StorageClient(std::shared_ptr<Channel> channel) 
			: stub_(StorageQuery::NewStub(channel)) {}

	std::string Get(const std::string& row, const std::string& col) {
		// Data we are sending to the server.
		GetRequest request;
		request.set_row(row);
		request.set_col(col);

		// Container for the data we expect from the server.
		GetResponse response;

		// Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
		ClientContext context;

		// The actual RPC.
    Status status = stub_->Get(&context, request, &response);

    // Act upon its status.
    if (status.ok()) {
      return response.val();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "Not Found!";
    }
	}

	void Put(const std::string& row, const std::string& col, const std::string& val) {
		// Data we are sending to the server.
		PutRequest request;
		request.set_row(row);
		request.set_col(col);
		request.set_val(val);

		// Container for the data we expect from the server.
		PutResponse response;

		// Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
		ClientContext context;

		// The actual RPC.
    Status status = stub_->Put(&context, request, &response);

    // Act upon its status.
    if (status.ok()) {
      return;
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return;
    }
	}

	void CPut(const std::string& row, const std::string& col, 
						const std::string& val1, const std::string& val2) {
		// Data we are sending to the server.
		CPutRequest request;
		request.set_row(row);
		request.set_col(col);
		request.set_val1(val1);
		request.set_val2(val2);

		// Container for the data we expect from the server.
		CPutResponse response;

		// Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
		ClientContext context;

		// The actual RPC.
    Status status = stub_->CPut(&context, request, &response);

    // Act upon its status.
    if (status.ok()) {
      return;
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return;
    }
	}

	void Delete(const std::string& row, const std::string& col) {
		// Data we are sending to the server.
		DeleteRequest request;
		request.set_row(row);
		request.set_col(col);

		// Container for the data we expect from the server.
		DeleteResponse response;

		// Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
		ClientContext context;

		// The actual RPC.
    Status status = stub_->Delete(&context, request, &response);

    // Act upon its status.
    if (status.ok()) {
      return;
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return;
    }
	}

private:
  std::unique_ptr<StorageQuery::Stub> stub_;
	
};

int main(int argc, char** argv) {	
	// TODO:
	// Instantiate the client. It requires a channel, out of which the actual RPCs
  // are created. This channel models a connection to an endpoint (in this case,
  // localhost at port 50051). We indicate that the channel isn't authenticated
  // (use of InsecureChannelCredentials()).
  StorageClient client(grpc::CreateChannel(
      "localhost:50051", grpc::InsecureChannelCredentials()));

  std::string row("lisa");
  std::string col("emails");
  std::string val("from 1 to 2:xxx");
  client.Put(row, col, val);
  std::cout << "putting lisa||emails||from 1 to 2:xxx" << std::endl;

  std::string response = client.Get(row, col);
  std::cout << "getting lisa||emails: " << response << std::endl;

  std::string val_new("from 1 to 2:ooo");
  client.CPut(row, col, val_new, val_new);
  std::cout << "cputting lisa||emails||from 1 to 2:ooo||from 1 to 2:ooo" << std::endl;

  response = client.Get(row, col);
  std::cout << "getting lisa||emails: " << response << std::endl;

  client.CPut(row, col, val, val_new);
  std::cout << "cputting lisa||emails||from 1 to 2:xxx||from 1 to 2:ooo" << std::endl;

  response = client.Get(row, col);
  std::cout << "getting lisa||emails: " << response << std::endl;

  client.Delete(row, col);
  std::cout << "deleting lisa||emails: " << response << std::endl;

	response = client.Get(row, col);
  std::cout << "getting lisa||emails: " << response << std::endl;

  return 0;
}