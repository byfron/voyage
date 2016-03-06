#pragma once

//TODO: separate in SERVER/CLIENT ?
//TODO: Define protobuf for every message
enum Handlers {
	SERVER_LOGIN_REQ_HANDLER,
	CLIENT_LOGIN_HANDLER, // handles login/logou
	SERVER_USER_ACTION_HANDLER, 
	CLIENT_UPDATE_HANDLER, //this handles several msgs: chat, content, move, ...
	CLIENT_MAP_HANDLER,
	SERVER_MAP_REQ_HANDLER,
	SERVER_DOWNLOAD_REQ_HANDLER,
	CLIENT_DOWNLOAD_DATA_HANDLER,
	CLIENT_DOWNLOAD_MEDIA_HANDLER
};
