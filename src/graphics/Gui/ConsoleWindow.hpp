#pragma once

class ConsoleWindow {

	void display();

	//whenever the player types we should try to auto-complete verbs and sustantives
	//looking at the actions/entities table (check how this is implemented)

	//matching verbs should be marked bold
	//matching sustantives should be bold colored like in description?
	void onType();


//	TextCompletionEngine::Ptr _textCompletion;

};
