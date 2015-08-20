#include <lucid/core/Identity.h>
#include <rpc.h>

#pragma comment (lib, "rpcrt4.lib")

///
///
///
namespace lucid {
namespace core {

	uint8_t const nullData[Identity::SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
	Identity const Identity::null(nullData);

	Identity::Identity()
	{
		RPC_STATUS status;

		///	by default a new id is generated...
		::UuidCreate((UUID*)_data);
		_hash = ::UuidHash((UUID*)_data, &status);
	}

	Identity::Identity(uint8_t const *data)
	{
		RPC_STATUS status;

		::memcpy(_data, data, SIZE);
		_hash = ::UuidHash((UUID*)_data, &status);
	}

	Identity::~Identity()
	{
	}

}	///	core
}	///	lucid