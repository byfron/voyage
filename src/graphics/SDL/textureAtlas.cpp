
int TextureAtlas::loadTexture(std::string file) {

	_surface = IMG_Load(GRAPHICS_DIR file.c_str());

	if (_surface == NULL) std::cout << "Error loading image:" << IMG_GetError() << std::endl;

	//create texture
	_textureId = 0;
	glGenTextures(1, &_textureId);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	int Mode = GL_RGB;
 
	if(_surface->format->BytesPerPixel == 4) {
		Mode = GL_RGBA;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, Mode, _surface->w, _surface->h, 0, Mode, GL_UNSIGNED_BYTE, _surface->pixels);
}

void TextureAtlas::renderSprite(int spriteId, float X, float Y) {

	float SrcX = 0;
	float SrcY = 0;
	
// Bind the texture to which subsequent calls refer
	//set rendering target to frame_buffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _textureId, 0);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	
	float tX = (SrcX / (float)_textureWidth);
	float tY = (SrcY / (float)_textureHeight);
	
	float tX2 = ((SrcX + _spriteWidth) / (float)_textureWidth);
	float tY2 = ((SrcY + _spriteHeight) / (float)_textureHeight);
	
//This combination is matched with the current Ortho view
//Don't change the Ortho view, otherwise textures will be flipped
//or rotated
	glBegin(GL_QUADS);
	
	glTexCoord2f(tX, tY);
	glVertex3f(X, Y, 0);
	
	glTexCoord2f(tX2, tY);
	glVertex3f(X + _spriteWidth, Y, 0);
	
	glTexCoord2f(tX2, tY2);
	glVertex3f(X + _spriteWidth, Y + _spriteHeight, 0);
	
	glTexCoord2f(tX, tY2);
	glVertex3f(X, Y + _spriteHeight, 0);
	
	glEnd();
	
}
