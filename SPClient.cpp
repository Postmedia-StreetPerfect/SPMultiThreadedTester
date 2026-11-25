#include <mutex>
#include <ranges>
#include <string>
#include <string_view>

#include "CStringOut.h"
#include "StreetPerfectInterface.h"
#include "SPClient.h"

std::mutex StreetPerfectNativeClient::_mtx;

char GS_XXX_out_null_string_15[100 + 1] = { "" };
char GS_XXX_out_null_string_16[100 + 1] = { "" };
char GS_XXX_out_null_string_17[100 + 1] = { "" };
char GS_XXX_out_null_string_18[100 + 1] = { "" };
char GS_XXX_out_null_string_19[100 + 1] = { "" };
char GS_XXX_out_null_string_20[100 + 1] = { "" };
char GS_XXX_out_null_string_21[100 + 1] = { "" };
char GS_XXX_out_null_string_22[100 + 1] = { "" };
char GS_XXX_out_null_string_23[100 + 1] = { "" };
char GS_XXX_out_null_string_24[100 + 1] = { "" };
char GS_XXX_out_null_string_25[100 + 1] = { "" };
char GS_XXX_out_null_string_26[100 + 1] = { "" };

//	std::mutex mtx;
ConnectionResponse StreetPerfectNativeClient::Connect(const char *constr)
{
	std::lock_guard<std::mutex> lock(_mtx);
	CStringOut status_flag_out(100);
	CStringOut status_messages_out(132);
	CStringOut in_parameter_file(2048, constr == nullptr ? "StreetPerfectAddressAccuracy.Ini" : constr);

	int ret = StreetPerfectConnect(in_parameter_file, status_flag_out, status_messages_out);
	ConnectionResponse r;
	r.ret = ret;
	r.status_flag = status_flag_out.str();
	r.status_messages = status_messages_out.str();
	return r;
}

ConnectionResponse StreetPerfectNativeClient::Disconnect()
{
	std::lock_guard<std::mutex> lock(_mtx);
	CStringOut status_flag_out(100);
	CStringOut status_messages_out(132);
	CStringOut in_parameter_file(2048, "");

	int ret = StreetPerfectDisconnect(in_parameter_file, status_flag_out, status_messages_out);

	ConnectionResponse r;
	r.ret = ret;
	r.status_flag = status_flag_out.str();
	r.status_messages = status_messages_out.str();
	return r;
}

caCorrectionResponse StreetPerfectNativeClient::caCorrectAddress(caAddressRequest& req)
{

	CStringOut CAN_in_function(2048, "CAN_AddressCorrection");
	CStringOut CAN_in_reserved(100, "");

	CStringOut CAN_out_status_flag(100);
	CStringOut CAN_out_recipient(100);
	CStringOut CAN_out_reserved(100);
	CStringOut CAN_out_address_line(200);
	CStringOut CAN_out_city(100);
	CStringOut CAN_out_province(100);
	CStringOut CAN_out_postal_code(100);
	CStringOut CAN_out_extra_information(200);
	CStringOut CAN_out_unidentified_component(100);

	CStringOut CAN_out_status_messages(300);
	CStringOut CAN_out_function_messages(3000);

	// thread guard
	std::lock_guard<std::mutex> lock(_mtx);

	int ret = StreetPerfectProcessAddress(req.options.data(),
		CAN_in_function,
		req.recipient.data(),
		CAN_in_reserved,
		req.address_line.data(),
		req.city.data(),
		req.province.data(),
		req.postal_code.data(),
		CAN_out_status_flag,
		CAN_out_status_messages,
		CAN_out_function_messages,
		CAN_out_recipient,
		CAN_out_reserved,
		CAN_out_address_line,
		CAN_out_city,
		CAN_out_province,
		CAN_out_postal_code,
		CAN_out_extra_information,
		CAN_out_unidentified_component,
		GS_XXX_out_null_string_20,
		GS_XXX_out_null_string_21,
		GS_XXX_out_null_string_22,
		GS_XXX_out_null_string_23,
		GS_XXX_out_null_string_24,
		GS_XXX_out_null_string_25,
		GS_XXX_out_null_string_26);

	caCorrectionResponse r;
	r.ret = ret;
	r.status_flag = CAN_out_status_flag.str();
	r.status_messages = CAN_out_status_messages.str();

	r.address_line = CAN_out_address_line.str();
	r.city = CAN_out_city.str();
	r.postal_code = CAN_out_postal_code.str();
	r.province = CAN_out_province.str();
	
	r.extra_information = CAN_out_extra_information.str();
	r.unidentified_component = CAN_out_unidentified_component.str();
	
	r.function_messages = splitCRLF(CAN_out_function_messages.cstr());

	return r;
}

// helper
std::list<std::string> StreetPerfectNativeClient::splitCRLF(const char* buf) 
{
	std::list<std::string> result;
	if (!buf) return result;

	using namespace std::literals;
	for (auto const &part : std::string_view(buf) | std::views::split("\r\n"sv)) {
		result.emplace_back(part.begin(), part.end());
	}

	return result;
}