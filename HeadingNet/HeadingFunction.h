#pragma once
namespace Heading
{

	template<typename ... Args>
	static std::string formatf	( _In_ const	std::string&		_format
								, _In_			Args ...			_args			)
	{
		int length = std::snprintf( nullptr, 0, _format.c_str(), _args ... ) + 1;

		if( length <= 0 )
		{
			return "";
		}

		std::string result = "";

		result.resize(length, '\0');

		std::snprintf( (char*)result.c_str(), length, ( char* )_format.c_str(), _args ... );

		return result;
	}

	static std::string PrintMem	( _In_			char*				_ptr
								, _In_			uint64_t			_length			);

	//=====================================================================================================================
	 
	// WSAStartUp 등의 초기화 함수
	static void		start		( _Out_			WSAData&			_data			);
	static void		end			(				void								);

	//=====================================================================================================================

	static bool		createInfo	( _Inout_		connectionInfo&		_connInfo		);
	static bool		connect		( _Inout_		connectionInfo&		_connInfo		);
	static bool		disconnect	( _Inout_		connectionInfo&		_connInfo		);

	static int		send		( _Inout_		connectionInfo&		_connInfo		);
	static int		recv		( _Inout_		connectionInfo&		_connInfo		);

	static int		Recorver	( _Inout_		connectionInfo&		_connInfo		);

	//=====================================================================================================================

	static bool		createInfo	( _Inout_		bindingInfo&		_connInfo		);
	static bool		bind		( _Inout_		bindingInfo&		_connInfo		);
	static int		aSelect		( _Inout_		bindingInfo&		_connInfo		);
	static bool		release		( _Inout_		bindingInfo&		_connInfo		);

	static bool		accept		( _Inout_		bindingInfo&		_connInfo		);
	static int		send		( _Inout_		bindingInfo&		_connInfo		);
	static int		recv		( _Inout_		bindingInfo&		_connInfo		);

	static int		Recorver	( _Inout_		bindingInfo&		_connInfo		);

	//=====================================================================================================================

	static Header*	Parsing		( _In_			Buffer&				_buffer			);
	static void		Parsing		( _In_			packetBuff&			_pullParsing	);
}