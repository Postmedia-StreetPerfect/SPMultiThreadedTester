#pragma once
#include <string>
#include <list>
#include <mutex>


class caAddressRequest
{
public:
	std::string options;

	std::string recipient;
	std::string address_line;
	std::string city;
	std::string province;
	std::string postal_code;
};

class caCorrectionResponse
{
public:
	caCorrectionResponse() {
		ret = -1;
	}
	std::string recipient;
	std::string address_line;
	std::string city;
	std::string province;
	std::string postal_code;
	std::string extra_information;
	std::string unidentified_component;

	/// <summary>
	/// * V = Submitted address is Valid
	/// * C = Submitted address is Corrected
	/// * N = Submitted address is Not correct
	/// * F = Submitted address is Foreign
	/// </summary>

	std::string status_flag;
	std::string status_messages;

	int ret;
	std::list<std::string> function_messages;

	bool ok() {
		return ret==0 && status_flag.find_first_of("VC") != std::string::npos;
	}
};

class ConnectionResponse
{
public:
	ConnectionResponse() {
		ret = -1;
	}
	std::string status_flag;
	std::string status_messages;
	int ret;
	bool ok() {
		return ret == 0 && status_flag == "N";
	}
};

class StreetPerfectNativeClient
{
	static std::mutex _mtx;
	static std::list<std::string> splitCRLF(const char* buf);
public:
	static ConnectionResponse Connect(const char* constr);
	static ConnectionResponse Disconnect();
	static caCorrectionResponse caCorrectAddress(caAddressRequest& req);

};

