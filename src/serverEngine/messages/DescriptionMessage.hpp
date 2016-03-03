#pragma once
#include <networking/messages.hpp>

class FormattedMessage : public Message {
public:

private:
	
	//text with the format
	
	//There are some ${E_12}s in the forest. 12 it's the ID of the entity 12
	//of an entity that is known a-priori by the client
	//(we should request the server the entity table,
	//the attributes table and the actions table if the client is missing them)	
	std::string formatted_text;

	//rest of data already parsed

	std::vector<EntityID> _entities;  
	
	
};
