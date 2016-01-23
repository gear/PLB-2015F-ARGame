#include "TextRenderer.h"

TextRenderer::TextRenderer(){
	font = NULL;
}

TextRenderer::~TextRenderer(){
}

/* 初期化 */
int TextRenderer::initialize(const char *fontPath, int fontSize, Uint8 r, Uint8 g, Uint8 b, float screenWidth, float screenHeight){

	/* フォントを開く */
	this->fontSize = fontSize;
	font = TTF_OpenFont(fontPath, fontSize);
	if ( font == NULL ) {
		printf("フォント(%s)を開けません: %s\n", fontPath, TTF_GetError());
		return -1;
	}

	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	/* 色の設定 */
	color.r = r;		color.g = g;		color.b = b;

	/* 高速描画用のテーブルを作る */
	//memset(asciiCharacterTable, 0, sizeof(SDL_Surface*) * TEXTRENDERER_TABLE_SIZE);
	memset(asciiCharacterTextureTable, 0, sizeof(CharacterTexture) * TEXTRENDERER_TABLE_SIZE);
	//createASCIITable();
	glEnable(GL_TEXTURE_2D);
	createASCIITextureTable();
	glDisable(GL_TEXTURE_2D);

	return 0;
} // initialize

/* 解放 */
void TextRenderer::finalize(){
	for ( int i = 0; i < TEXTRENDERER_TABLE_SIZE; i++ ){
		if ( asciiCharacterTextureTable[i].textureNo ){
			glDeleteTextures(1, &asciiCharacterTextureTable[i].textureNo );
		}
	}
	if ( font ){
		TTF_CloseFont(font);
		font = NULL;
	}
} // finalize

/* Render ASCII Text(Centering enabled) */
void TextRenderer::renderASCII(const char *srcString, float x, float y, float margin, float xScale, float yScale){

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float currentX = x - screenWidth / 2.0f;
	y = -y + screenHeight / 2.0f;
	int width, height;

	glEnable(GL_TEXTURE_2D);
	for (int i = 0; srcString[i] != '\0'; i++){		

		width = asciiCharacterTextureTable[ srcString[i] ].width;
		height = asciiCharacterTextureTable[ srcString[i] ].height;

		glPushMatrix();

			glBindTexture(GL_TEXTURE_2D, asciiCharacterTextureTable[ srcString[i] ].textureNo);

			glScalef(xScale, yScale, 1.0f);
			glTranslatef(currentX, y, 1.0f);

			glBegin(GL_QUADS);
				glColor3f(1.0f, 1.0f, 1.0f);

				glTexCoord2f(0 , asciiCharacterTextureTable[ srcString[i] ].texCoordY);
				glVertex3f(-width / 2.0f, -height / 2.0f, 1.0f);

				glTexCoord2f(asciiCharacterTextureTable[ srcString[i] ].texCoordX , asciiCharacterTextureTable[ srcString[i] ].texCoordY);
				glVertex3f(width / 2.0f, -height / 2.0f, 1.0f);

				glTexCoord2f(asciiCharacterTextureTable[ srcString[i] ].texCoordX , 0);
				glVertex3f(width / 2.0f, height / 2.0f, 1.0f);

				glTexCoord2f(0 , 0);
				glVertex3f(-width / 2.0f, height / 2.0f, 1.0f);
			glEnd();

			glBindTexture(GL_TEXTURE_2D , 0);
	
		glPopMatrix();

		currentX += (width + margin) * xScale;				// 表示位置を文字の幅とmargin分ずらす

	} // for
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	glBlendFunc(GL_ONE, GL_ZERO);
	glDisable(GL_BLEND);
} // renderASCII

/* Render ASCII Text(Centering disabled) */
void TextRenderer::renderASCIICenter(const char *srcString, float y, float margin, float xScale, float yScale){

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Uint32 length = (Uint32)strlen(srcString);
	float currentX = -( asciiCharacterTextureTable[ '?' ].width * length + margin * (length - 1) ) / 2.0f;			// xが中央になるように開始位置をずらす(あってる？)

	y = -y + screenHeight / 2.0f;

	int width, height;

	glEnable(GL_TEXTURE_2D);
	for (int i = 0; srcString[i] != '\0'; i++){		

		width = asciiCharacterTextureTable[ srcString[i] ].width;
		height = asciiCharacterTextureTable[ srcString[i] ].height;

		glPushMatrix();

			glBindTexture(GL_TEXTURE_2D , asciiCharacterTextureTable[ srcString[i] ].textureNo);

			glScalef(xScale, yScale, 1.0f);
			glTranslatef(currentX, y, 1.0f);

			glBegin(GL_QUADS);
				glColor3f(1.0f, 1.0f, 1.0f);

				glTexCoord2f(0 , asciiCharacterTextureTable[ srcString[i] ].texCoordY);
				glVertex3f(-width / 2.0f, -height / 2.0f, 1.0f);

				glTexCoord2f(asciiCharacterTextureTable[ srcString[i] ].texCoordX , asciiCharacterTextureTable[ srcString[i] ].texCoordY);
				glVertex3f(width / 2.0f, -height / 2.0f, 1.0f);

				glTexCoord2f(asciiCharacterTextureTable[ srcString[i] ].texCoordX , 0);
				glVertex3f(width / 2.0f, height / 2.0f, 1.0f);

				glTexCoord2f(0 , 0);
				glVertex3f(-width / 2.0f, height / 2.0f, 1.0f);
			glEnd();

			glBindTexture(GL_TEXTURE_2D , 0);
	
		glPopMatrix();

		currentX += (width + margin) * xScale;				// 表示位置を文字の幅とmargin分ずらす

	} // for
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	glBlendFunc(GL_ONE, GL_ZERO);
	glDisable(GL_BLEND);
} // renderASCIICenter


/* Create Text Image */
ImageRenderer *TextRenderer::createTextImage(const char *string, int fontSize, Uint8 r, Uint8 g, Uint8 b){
	if ( font ){
		ImageRenderer *newImageRenderer = new ImageRenderer();
		SDL_Color color = {r, g, b, 0};
		float texCoordX, texCoordY;
		SDL_Surface *newSurface = loadText(string, color, &texCoordX, &texCoordY);
		int width, height;
		GLuint newTextureNo = createTexture( newSurface, &width, &height );
		newImageRenderer->setTexture(newTextureNo, width, height, texCoordX, texCoordY, 1);
		return newImageRenderer;
	}else{
		printf("Fontが開かれていません。\n");
		return NULL;
	}
} // createTextImage

/* 必要に応じてフォントを閉じる(作成したテーブルを使った文字表示のみで、今後createTextImage()等を必要としない場合にフォントのみを閉じる) */
void TextRenderer::closeFont(){
	TTF_CloseFont(font);
	font = NULL;
} // closeFont

/* 高速描画用文字テーブルを作る */
void TextRenderer::createASCIITextureTable(){

	int width, height;
	int fontImageWidth, fontImageHeight;
	float texCoordX, texCoordY;
	asciiCharacterTextureTable['a'].textureNo = createTexture( loadCharacter('a', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['a'].width = fontImageWidth;	asciiCharacterTextureTable['a'].height = fontImageHeight;	asciiCharacterTextureTable['a'].texCoordX = texCoordX;	asciiCharacterTextureTable['a'].texCoordY = texCoordY;
	asciiCharacterTextureTable['b'].textureNo = createTexture( loadCharacter('b', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['b'].width = fontImageWidth;	asciiCharacterTextureTable['b'].height = fontImageHeight;	asciiCharacterTextureTable['b'].texCoordX = texCoordX;	asciiCharacterTextureTable['b'].texCoordY = texCoordY;
	asciiCharacterTextureTable['c'].textureNo = createTexture( loadCharacter('c', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['c'].width = fontImageWidth;	asciiCharacterTextureTable['c'].height = fontImageHeight;	asciiCharacterTextureTable['c'].texCoordX = texCoordX;	asciiCharacterTextureTable['c'].texCoordY = texCoordY;
	asciiCharacterTextureTable['d'].textureNo = createTexture( loadCharacter('d', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['d'].width = fontImageWidth;	asciiCharacterTextureTable['d'].height = fontImageHeight;	asciiCharacterTextureTable['d'].texCoordX = texCoordX;	asciiCharacterTextureTable['d'].texCoordY = texCoordY;
	asciiCharacterTextureTable['e'].textureNo = createTexture( loadCharacter('e', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['e'].width = fontImageWidth;	asciiCharacterTextureTable['e'].height = fontImageHeight;	asciiCharacterTextureTable['e'].texCoordX = texCoordX;	asciiCharacterTextureTable['e'].texCoordY = texCoordY;
	asciiCharacterTextureTable['f'].textureNo = createTexture( loadCharacter('f', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['f'].width = fontImageWidth;	asciiCharacterTextureTable['f'].height = fontImageHeight;	asciiCharacterTextureTable['f'].texCoordX = texCoordX;	asciiCharacterTextureTable['f'].texCoordY = texCoordY;
	asciiCharacterTextureTable['g'].textureNo = createTexture( loadCharacter('g', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['g'].width = fontImageWidth;	asciiCharacterTextureTable['g'].height = fontImageHeight;	asciiCharacterTextureTable['g'].texCoordX = texCoordX;	asciiCharacterTextureTable['g'].texCoordY = texCoordY;
	asciiCharacterTextureTable['h'].textureNo = createTexture( loadCharacter('h', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['h'].width = fontImageWidth;	asciiCharacterTextureTable['h'].height = fontImageHeight;	asciiCharacterTextureTable['h'].texCoordX = texCoordX;	asciiCharacterTextureTable['h'].texCoordY = texCoordY;
	asciiCharacterTextureTable['i'].textureNo = createTexture( loadCharacter('i', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['i'].width = fontImageWidth;	asciiCharacterTextureTable['i'].height = fontImageHeight;	asciiCharacterTextureTable['i'].texCoordX = texCoordX;	asciiCharacterTextureTable['i'].texCoordY = texCoordY;
	asciiCharacterTextureTable['j'].textureNo = createTexture( loadCharacter('j', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['j'].width = fontImageWidth;	asciiCharacterTextureTable['j'].height = fontImageHeight;	asciiCharacterTextureTable['j'].texCoordX = texCoordX;	asciiCharacterTextureTable['j'].texCoordY = texCoordY;
	asciiCharacterTextureTable['k'].textureNo = createTexture( loadCharacter('k', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['k'].width = fontImageWidth;	asciiCharacterTextureTable['k'].height = fontImageHeight;	asciiCharacterTextureTable['k'].texCoordX = texCoordX;	asciiCharacterTextureTable['k'].texCoordY = texCoordY;
	asciiCharacterTextureTable['l'].textureNo = createTexture( loadCharacter('l', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['l'].width = fontImageWidth;	asciiCharacterTextureTable['l'].height = fontImageHeight;	asciiCharacterTextureTable['l'].texCoordX = texCoordX;	asciiCharacterTextureTable['l'].texCoordY = texCoordY;
	asciiCharacterTextureTable['m'].textureNo = createTexture( loadCharacter('m', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['m'].width = fontImageWidth;	asciiCharacterTextureTable['m'].height = fontImageHeight;	asciiCharacterTextureTable['m'].texCoordX = texCoordX;	asciiCharacterTextureTable['m'].texCoordY = texCoordY;
	asciiCharacterTextureTable['n'].textureNo = createTexture( loadCharacter('n', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['n'].width = fontImageWidth;	asciiCharacterTextureTable['n'].height = fontImageHeight;	asciiCharacterTextureTable['n'].texCoordX = texCoordX;	asciiCharacterTextureTable['n'].texCoordY = texCoordY;
	asciiCharacterTextureTable['o'].textureNo = createTexture( loadCharacter('o', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['o'].width = fontImageWidth;	asciiCharacterTextureTable['o'].height = fontImageHeight;	asciiCharacterTextureTable['o'].texCoordX = texCoordX;	asciiCharacterTextureTable['o'].texCoordY = texCoordY;
	asciiCharacterTextureTable['p'].textureNo = createTexture( loadCharacter('p', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['p'].width = fontImageWidth;	asciiCharacterTextureTable['p'].height = fontImageHeight;	asciiCharacterTextureTable['p'].texCoordX = texCoordX;	asciiCharacterTextureTable['p'].texCoordY = texCoordY;
	asciiCharacterTextureTable['q'].textureNo = createTexture( loadCharacter('q', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['q'].width = fontImageWidth;	asciiCharacterTextureTable['q'].height = fontImageHeight;	asciiCharacterTextureTable['q'].texCoordX = texCoordX;	asciiCharacterTextureTable['q'].texCoordY = texCoordY;
	asciiCharacterTextureTable['r'].textureNo = createTexture( loadCharacter('r', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['r'].width = fontImageWidth;	asciiCharacterTextureTable['r'].height = fontImageHeight;	asciiCharacterTextureTable['r'].texCoordX = texCoordX;	asciiCharacterTextureTable['r'].texCoordY = texCoordY;
	asciiCharacterTextureTable['s'].textureNo = createTexture( loadCharacter('s', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['s'].width = fontImageWidth;	asciiCharacterTextureTable['s'].height = fontImageHeight;	asciiCharacterTextureTable['s'].texCoordX = texCoordX;	asciiCharacterTextureTable['s'].texCoordY = texCoordY;
	asciiCharacterTextureTable['t'].textureNo = createTexture( loadCharacter('t', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['t'].width = fontImageWidth;	asciiCharacterTextureTable['t'].height = fontImageHeight;	asciiCharacterTextureTable['t'].texCoordX = texCoordX;	asciiCharacterTextureTable['t'].texCoordY = texCoordY;
	asciiCharacterTextureTable['u'].textureNo = createTexture( loadCharacter('u', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['u'].width = fontImageWidth;	asciiCharacterTextureTable['u'].height = fontImageHeight;	asciiCharacterTextureTable['u'].texCoordX = texCoordX;	asciiCharacterTextureTable['u'].texCoordY = texCoordY;
	asciiCharacterTextureTable['v'].textureNo = createTexture( loadCharacter('v', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['v'].width = fontImageWidth;	asciiCharacterTextureTable['v'].height = fontImageHeight;	asciiCharacterTextureTable['v'].texCoordX = texCoordX;	asciiCharacterTextureTable['v'].texCoordY = texCoordY;
	asciiCharacterTextureTable['w'].textureNo = createTexture( loadCharacter('w', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['w'].width = fontImageWidth;	asciiCharacterTextureTable['w'].height = fontImageHeight;	asciiCharacterTextureTable['w'].texCoordX = texCoordX;	asciiCharacterTextureTable['w'].texCoordY = texCoordY;
	asciiCharacterTextureTable['x'].textureNo = createTexture( loadCharacter('x', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['x'].width = fontImageWidth;	asciiCharacterTextureTable['x'].height = fontImageHeight;	asciiCharacterTextureTable['x'].texCoordX = texCoordX;	asciiCharacterTextureTable['x'].texCoordY = texCoordY;
	asciiCharacterTextureTable['y'].textureNo = createTexture( loadCharacter('y', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['y'].width = fontImageWidth;	asciiCharacterTextureTable['y'].height = fontImageHeight;	asciiCharacterTextureTable['y'].texCoordX = texCoordX;	asciiCharacterTextureTable['y'].texCoordY = texCoordY;
	asciiCharacterTextureTable['z'].textureNo = createTexture( loadCharacter('z', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['z'].width = fontImageWidth;	asciiCharacterTextureTable['z'].height = fontImageHeight;	asciiCharacterTextureTable['z'].texCoordX = texCoordX;	asciiCharacterTextureTable['z'].texCoordY = texCoordY;

	asciiCharacterTextureTable['A'].textureNo = createTexture( loadCharacter('A', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['A'].width = fontImageWidth;	asciiCharacterTextureTable['A'].height = fontImageHeight;	asciiCharacterTextureTable['A'].texCoordX = texCoordX;	asciiCharacterTextureTable['A'].texCoordY = texCoordY;
	asciiCharacterTextureTable['B'].textureNo = createTexture( loadCharacter('B', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['B'].width = fontImageWidth;	asciiCharacterTextureTable['B'].height = fontImageHeight;	asciiCharacterTextureTable['B'].texCoordX = texCoordX;	asciiCharacterTextureTable['B'].texCoordY = texCoordY;
	asciiCharacterTextureTable['C'].textureNo = createTexture( loadCharacter('C', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['C'].width = fontImageWidth;	asciiCharacterTextureTable['C'].height = fontImageHeight;	asciiCharacterTextureTable['C'].texCoordX = texCoordX;	asciiCharacterTextureTable['C'].texCoordY = texCoordY;
	asciiCharacterTextureTable['D'].textureNo = createTexture( loadCharacter('D', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['D'].width = fontImageWidth;	asciiCharacterTextureTable['D'].height = fontImageHeight;	asciiCharacterTextureTable['D'].texCoordX = texCoordX;	asciiCharacterTextureTable['D'].texCoordY = texCoordY;
	asciiCharacterTextureTable['E'].textureNo = createTexture( loadCharacter('E', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['E'].width = fontImageWidth;	asciiCharacterTextureTable['E'].height = fontImageHeight;	asciiCharacterTextureTable['E'].texCoordX = texCoordX;	asciiCharacterTextureTable['E'].texCoordY = texCoordY;
	asciiCharacterTextureTable['F'].textureNo = createTexture( loadCharacter('F', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['F'].width = fontImageWidth;	asciiCharacterTextureTable['F'].height = fontImageHeight;	asciiCharacterTextureTable['F'].texCoordX = texCoordX;	asciiCharacterTextureTable['F'].texCoordY = texCoordY;
	asciiCharacterTextureTable['G'].textureNo = createTexture( loadCharacter('G', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['G'].width = fontImageWidth;	asciiCharacterTextureTable['G'].height = fontImageHeight;	asciiCharacterTextureTable['G'].texCoordX = texCoordX;	asciiCharacterTextureTable['G'].texCoordY = texCoordY;
	asciiCharacterTextureTable['H'].textureNo = createTexture( loadCharacter('H', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['H'].width = fontImageWidth;	asciiCharacterTextureTable['H'].height = fontImageHeight;	asciiCharacterTextureTable['H'].texCoordX = texCoordX;	asciiCharacterTextureTable['H'].texCoordY = texCoordY;
	asciiCharacterTextureTable['I'].textureNo = createTexture( loadCharacter('I', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['I'].width = fontImageWidth;	asciiCharacterTextureTable['I'].height = fontImageHeight;	asciiCharacterTextureTable['I'].texCoordX = texCoordX;	asciiCharacterTextureTable['I'].texCoordY = texCoordY;
	asciiCharacterTextureTable['J'].textureNo = createTexture( loadCharacter('J', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['J'].width = fontImageWidth;	asciiCharacterTextureTable['J'].height = fontImageHeight;	asciiCharacterTextureTable['J'].texCoordX = texCoordX;	asciiCharacterTextureTable['J'].texCoordY = texCoordY;
	asciiCharacterTextureTable['K'].textureNo = createTexture( loadCharacter('K', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['K'].width = fontImageWidth;	asciiCharacterTextureTable['K'].height = fontImageHeight;	asciiCharacterTextureTable['K'].texCoordX = texCoordX;	asciiCharacterTextureTable['K'].texCoordY = texCoordY;
	asciiCharacterTextureTable['L'].textureNo = createTexture( loadCharacter('L', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['L'].width = fontImageWidth;	asciiCharacterTextureTable['L'].height = fontImageHeight;	asciiCharacterTextureTable['L'].texCoordX = texCoordX;	asciiCharacterTextureTable['L'].texCoordY = texCoordY;
	asciiCharacterTextureTable['M'].textureNo = createTexture( loadCharacter('M', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['M'].width = fontImageWidth;	asciiCharacterTextureTable['M'].height = fontImageHeight;	asciiCharacterTextureTable['M'].texCoordX = texCoordX;	asciiCharacterTextureTable['M'].texCoordY = texCoordY;
	asciiCharacterTextureTable['N'].textureNo = createTexture( loadCharacter('N', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['N'].width = fontImageWidth;	asciiCharacterTextureTable['N'].height = fontImageHeight;	asciiCharacterTextureTable['N'].texCoordX = texCoordX;	asciiCharacterTextureTable['N'].texCoordY = texCoordY;
	asciiCharacterTextureTable['O'].textureNo = createTexture( loadCharacter('O', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['O'].width = fontImageWidth;	asciiCharacterTextureTable['O'].height = fontImageHeight;	asciiCharacterTextureTable['O'].texCoordX = texCoordX;	asciiCharacterTextureTable['O'].texCoordY = texCoordY;
	asciiCharacterTextureTable['P'].textureNo = createTexture( loadCharacter('P', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['P'].width = fontImageWidth;	asciiCharacterTextureTable['P'].height = fontImageHeight;	asciiCharacterTextureTable['P'].texCoordX = texCoordX;	asciiCharacterTextureTable['P'].texCoordY = texCoordY;
	asciiCharacterTextureTable['Q'].textureNo = createTexture( loadCharacter('Q', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['Q'].width = fontImageWidth;	asciiCharacterTextureTable['Q'].height = fontImageHeight;	asciiCharacterTextureTable['Q'].texCoordX = texCoordX;	asciiCharacterTextureTable['Q'].texCoordY = texCoordY;
	asciiCharacterTextureTable['R'].textureNo = createTexture( loadCharacter('R', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['R'].width = fontImageWidth;	asciiCharacterTextureTable['R'].height = fontImageHeight;	asciiCharacterTextureTable['R'].texCoordX = texCoordX;	asciiCharacterTextureTable['R'].texCoordY = texCoordY;
	asciiCharacterTextureTable['S'].textureNo = createTexture( loadCharacter('S', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['S'].width = fontImageWidth;	asciiCharacterTextureTable['S'].height = fontImageHeight;	asciiCharacterTextureTable['S'].texCoordX = texCoordX;	asciiCharacterTextureTable['S'].texCoordY = texCoordY;
	asciiCharacterTextureTable['T'].textureNo = createTexture( loadCharacter('T', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['T'].width = fontImageWidth;	asciiCharacterTextureTable['T'].height = fontImageHeight;	asciiCharacterTextureTable['T'].texCoordX = texCoordX;	asciiCharacterTextureTable['T'].texCoordY = texCoordY;
	asciiCharacterTextureTable['U'].textureNo = createTexture( loadCharacter('U', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['U'].width = fontImageWidth;	asciiCharacterTextureTable['U'].height = fontImageHeight;	asciiCharacterTextureTable['U'].texCoordX = texCoordX;	asciiCharacterTextureTable['U'].texCoordY = texCoordY;
	asciiCharacterTextureTable['V'].textureNo = createTexture( loadCharacter('V', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['V'].width = fontImageWidth;	asciiCharacterTextureTable['V'].height = fontImageHeight;	asciiCharacterTextureTable['V'].texCoordX = texCoordX;	asciiCharacterTextureTable['V'].texCoordY = texCoordY;
	asciiCharacterTextureTable['W'].textureNo = createTexture( loadCharacter('W', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['W'].width = fontImageWidth;	asciiCharacterTextureTable['W'].height = fontImageHeight;	asciiCharacterTextureTable['W'].texCoordX = texCoordX;	asciiCharacterTextureTable['W'].texCoordY = texCoordY;
	asciiCharacterTextureTable['X'].textureNo = createTexture( loadCharacter('X', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['X'].width = fontImageWidth;	asciiCharacterTextureTable['X'].height = fontImageHeight;	asciiCharacterTextureTable['X'].texCoordX = texCoordX;	asciiCharacterTextureTable['X'].texCoordY = texCoordY;
	asciiCharacterTextureTable['Y'].textureNo = createTexture( loadCharacter('Y', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['Y'].width = fontImageWidth;	asciiCharacterTextureTable['Y'].height = fontImageHeight;	asciiCharacterTextureTable['Y'].texCoordX = texCoordX;	asciiCharacterTextureTable['Y'].texCoordY = texCoordY;
	asciiCharacterTextureTable['Z'].textureNo = createTexture( loadCharacter('Z', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['Z'].width = fontImageWidth;	asciiCharacterTextureTable['Z'].height = fontImageHeight;	asciiCharacterTextureTable['Z'].texCoordX = texCoordX;	asciiCharacterTextureTable['Z'].texCoordY = texCoordY;

	asciiCharacterTextureTable['0'].textureNo = createTexture( loadCharacter('0', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['0'].width = fontImageWidth;	asciiCharacterTextureTable['0'].height = fontImageHeight;	asciiCharacterTextureTable['0'].texCoordX = texCoordX;	asciiCharacterTextureTable['0'].texCoordY = texCoordY;
	asciiCharacterTextureTable['1'].textureNo = createTexture( loadCharacter('1', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['1'].width = fontImageWidth;	asciiCharacterTextureTable['1'].height = fontImageHeight;	asciiCharacterTextureTable['1'].texCoordX = texCoordX;	asciiCharacterTextureTable['1'].texCoordY = texCoordY;
	asciiCharacterTextureTable['2'].textureNo = createTexture( loadCharacter('2', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['2'].width = fontImageWidth;	asciiCharacterTextureTable['2'].height = fontImageHeight;	asciiCharacterTextureTable['2'].texCoordX = texCoordX;	asciiCharacterTextureTable['2'].texCoordY = texCoordY;
	asciiCharacterTextureTable['3'].textureNo = createTexture( loadCharacter('3', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['3'].width = fontImageWidth;	asciiCharacterTextureTable['3'].height = fontImageHeight;	asciiCharacterTextureTable['3'].texCoordX = texCoordX;	asciiCharacterTextureTable['3'].texCoordY = texCoordY;
	asciiCharacterTextureTable['4'].textureNo = createTexture( loadCharacter('4', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['4'].width = fontImageWidth;	asciiCharacterTextureTable['4'].height = fontImageHeight;	asciiCharacterTextureTable['4'].texCoordX = texCoordX;	asciiCharacterTextureTable['4'].texCoordY = texCoordY;
	asciiCharacterTextureTable['5'].textureNo = createTexture( loadCharacter('5', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['5'].width = fontImageWidth;	asciiCharacterTextureTable['5'].height = fontImageHeight;	asciiCharacterTextureTable['5'].texCoordX = texCoordX;	asciiCharacterTextureTable['5'].texCoordY = texCoordY;
	asciiCharacterTextureTable['6'].textureNo = createTexture( loadCharacter('6', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['6'].width = fontImageWidth;	asciiCharacterTextureTable['6'].height = fontImageHeight;	asciiCharacterTextureTable['6'].texCoordX = texCoordX;	asciiCharacterTextureTable['6'].texCoordY = texCoordY;
	asciiCharacterTextureTable['7'].textureNo = createTexture( loadCharacter('7', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['7'].width = fontImageWidth;	asciiCharacterTextureTable['7'].height = fontImageHeight;	asciiCharacterTextureTable['7'].texCoordX = texCoordX;	asciiCharacterTextureTable['7'].texCoordY = texCoordY;
	asciiCharacterTextureTable['8'].textureNo = createTexture( loadCharacter('8', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['8'].width = fontImageWidth;	asciiCharacterTextureTable['8'].height = fontImageHeight;	asciiCharacterTextureTable['8'].texCoordX = texCoordX;	asciiCharacterTextureTable['8'].texCoordY = texCoordY;
	asciiCharacterTextureTable['9'].textureNo = createTexture( loadCharacter('9', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['9'].width = fontImageWidth;	asciiCharacterTextureTable['9'].height = fontImageHeight;	asciiCharacterTextureTable['9'].texCoordX = texCoordX;	asciiCharacterTextureTable['9'].texCoordY = texCoordY;

	asciiCharacterTextureTable['!'].textureNo = createTexture( loadCharacter('!', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['!'].width = fontImageWidth;	asciiCharacterTextureTable['!'].height = fontImageHeight;	asciiCharacterTextureTable['!'].texCoordX = texCoordX;	asciiCharacterTextureTable['!'].texCoordY = texCoordY;
	asciiCharacterTextureTable['"'].textureNo = createTexture( loadCharacter('"', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['"'].width = fontImageWidth;	asciiCharacterTextureTable['"'].height = fontImageHeight;	asciiCharacterTextureTable['"'].texCoordX = texCoordX;	asciiCharacterTextureTable['"'].texCoordY = texCoordY;
	asciiCharacterTextureTable['#'].textureNo = createTexture( loadCharacter('#', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['#'].width = fontImageWidth;	asciiCharacterTextureTable['#'].height = fontImageHeight;	asciiCharacterTextureTable['#'].texCoordX = texCoordX;	asciiCharacterTextureTable['#'].texCoordY = texCoordY;
	asciiCharacterTextureTable['$'].textureNo = createTexture( loadCharacter('$', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['$'].width = fontImageWidth;	asciiCharacterTextureTable['$'].height = fontImageHeight;	asciiCharacterTextureTable['$'].texCoordX = texCoordX;	asciiCharacterTextureTable['$'].texCoordY = texCoordY;
	asciiCharacterTextureTable['%'].textureNo = createTexture( loadCharacter('%', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['%'].width = fontImageWidth;	asciiCharacterTextureTable['%'].height = fontImageHeight;	asciiCharacterTextureTable['%'].texCoordX = texCoordX;	asciiCharacterTextureTable['%'].texCoordY = texCoordY;
	asciiCharacterTextureTable['&'].textureNo = createTexture( loadCharacter('&', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['&'].width = fontImageWidth;	asciiCharacterTextureTable['&'].height = fontImageHeight;	asciiCharacterTextureTable['&'].texCoordX = texCoordX;	asciiCharacterTextureTable['&'].texCoordY = texCoordY;
	asciiCharacterTextureTable['\''].textureNo = createTexture( loadCharacter('\'', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['\''].width = fontImageWidth;	asciiCharacterTextureTable['\''].height = fontImageHeight;	asciiCharacterTextureTable['\''].texCoordX = texCoordX;	asciiCharacterTextureTable['\''].texCoordY = texCoordY;
	asciiCharacterTextureTable[')'].textureNo = createTexture( loadCharacter(')', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable[')'].width = fontImageWidth;	asciiCharacterTextureTable[')'].height = fontImageHeight;	asciiCharacterTextureTable[')'].texCoordX = texCoordX;	asciiCharacterTextureTable[')'].texCoordY = texCoordY;
	asciiCharacterTextureTable['('].textureNo = createTexture( loadCharacter('(', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['('].width = fontImageWidth;	asciiCharacterTextureTable['('].height = fontImageHeight;	asciiCharacterTextureTable['('].texCoordX = texCoordX;	asciiCharacterTextureTable['('].texCoordY = texCoordY;
	asciiCharacterTextureTable['='].textureNo = createTexture( loadCharacter('=', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['='].width = fontImageWidth;	asciiCharacterTextureTable['='].height = fontImageHeight;	asciiCharacterTextureTable['='].texCoordX = texCoordX;	asciiCharacterTextureTable['='].texCoordY = texCoordY;
	asciiCharacterTextureTable['-'].textureNo = createTexture( loadCharacter('-', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['-'].width = fontImageWidth;	asciiCharacterTextureTable['-'].height = fontImageHeight;	asciiCharacterTextureTable['-'].texCoordX = texCoordX;	asciiCharacterTextureTable['-'].texCoordY = texCoordY;
	asciiCharacterTextureTable['~'].textureNo = createTexture( loadCharacter('~', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['~'].width = fontImageWidth;	asciiCharacterTextureTable['~'].height = fontImageHeight;	asciiCharacterTextureTable['~'].texCoordX = texCoordX;	asciiCharacterTextureTable['~'].texCoordY = texCoordY;
	asciiCharacterTextureTable['^'].textureNo = createTexture( loadCharacter('^', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['^'].width = fontImageWidth;	asciiCharacterTextureTable['^'].height = fontImageHeight;	asciiCharacterTextureTable['^'].texCoordX = texCoordX;	asciiCharacterTextureTable['^'].texCoordY = texCoordY;
	asciiCharacterTextureTable['|'].textureNo = createTexture( loadCharacter('|', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['|'].width = fontImageWidth;	asciiCharacterTextureTable['|'].height = fontImageHeight;	asciiCharacterTextureTable['|'].texCoordX = texCoordX;	asciiCharacterTextureTable['|'].texCoordY = texCoordY;
	asciiCharacterTextureTable['\\'].textureNo = createTexture( loadCharacter('\\', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['\\'].width = fontImageWidth;	asciiCharacterTextureTable['\\'].height = fontImageHeight;	asciiCharacterTextureTable['\\'].texCoordX = texCoordX;	asciiCharacterTextureTable['\\'].texCoordY = texCoordY;
	asciiCharacterTextureTable['+'].textureNo = createTexture( loadCharacter('+', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['+'].width = fontImageWidth;	asciiCharacterTextureTable['+'].height = fontImageHeight;	asciiCharacterTextureTable['+'].texCoordX = texCoordX;	asciiCharacterTextureTable['+'].texCoordY = texCoordY;
	asciiCharacterTextureTable[';'].textureNo = createTexture( loadCharacter(';', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable[';'].width = fontImageWidth;	asciiCharacterTextureTable[';'].height = fontImageHeight;	asciiCharacterTextureTable[';'].texCoordX = texCoordX;	asciiCharacterTextureTable[';'].texCoordY = texCoordY;
	asciiCharacterTextureTable[':'].textureNo = createTexture( loadCharacter(':', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable[':'].width = fontImageWidth;	asciiCharacterTextureTable[':'].height = fontImageHeight;	asciiCharacterTextureTable[':'].texCoordX = texCoordX;	asciiCharacterTextureTable[':'].texCoordY = texCoordY;
	asciiCharacterTextureTable['*'].textureNo = createTexture( loadCharacter('*', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['*'].width = fontImageWidth;	asciiCharacterTextureTable['*'].height = fontImageHeight;	asciiCharacterTextureTable['*'].texCoordX = texCoordX;	asciiCharacterTextureTable['*'].texCoordY = texCoordY;
	asciiCharacterTextureTable['['].textureNo = createTexture( loadCharacter('[', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['['].width = fontImageWidth;	asciiCharacterTextureTable['['].height = fontImageHeight;	asciiCharacterTextureTable['['].texCoordX = texCoordX;	asciiCharacterTextureTable['['].texCoordY = texCoordY;
	asciiCharacterTextureTable[']'].textureNo = createTexture( loadCharacter(']', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable[']'].width = fontImageWidth;	asciiCharacterTextureTable[']'].height = fontImageHeight;	asciiCharacterTextureTable[']'].texCoordX = texCoordX;	asciiCharacterTextureTable[']'].texCoordY = texCoordY;
	asciiCharacterTextureTable['{'].textureNo = createTexture( loadCharacter('{', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['{'].width = fontImageWidth;	asciiCharacterTextureTable['{'].height = fontImageHeight;	asciiCharacterTextureTable['{'].texCoordX = texCoordX;	asciiCharacterTextureTable['{'].texCoordY = texCoordY;
	asciiCharacterTextureTable['}'].textureNo = createTexture( loadCharacter('}', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['}'].width = fontImageWidth;	asciiCharacterTextureTable['}'].height = fontImageHeight;	asciiCharacterTextureTable['}'].texCoordX = texCoordX;	asciiCharacterTextureTable['}'].texCoordY = texCoordY;
	asciiCharacterTextureTable['<'].textureNo = createTexture( loadCharacter('<', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['<'].width = fontImageWidth;	asciiCharacterTextureTable['<'].height = fontImageHeight;	asciiCharacterTextureTable['<'].texCoordX = texCoordX;	asciiCharacterTextureTable['<'].texCoordY = texCoordY;
	asciiCharacterTextureTable['>'].textureNo = createTexture( loadCharacter('>', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['>'].width = fontImageWidth;	asciiCharacterTextureTable['>'].height = fontImageHeight;	asciiCharacterTextureTable['>'].texCoordX = texCoordX;	asciiCharacterTextureTable['>'].texCoordY = texCoordY;
	asciiCharacterTextureTable[','].textureNo = createTexture( loadCharacter(',', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable[','].width = fontImageWidth;	asciiCharacterTextureTable[','].height = fontImageHeight;	asciiCharacterTextureTable[','].texCoordX = texCoordX;	asciiCharacterTextureTable[','].texCoordY = texCoordY;
	asciiCharacterTextureTable['.'].textureNo = createTexture( loadCharacter('.', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['.'].width = fontImageWidth;	asciiCharacterTextureTable['.'].height = fontImageHeight;	asciiCharacterTextureTable['.'].texCoordX = texCoordX;	asciiCharacterTextureTable['.'].texCoordY = texCoordY;
	asciiCharacterTextureTable['?'].textureNo = createTexture( loadCharacter('?', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['?'].width = fontImageWidth;	asciiCharacterTextureTable['?'].height = fontImageHeight;	asciiCharacterTextureTable['?'].texCoordX = texCoordX;	asciiCharacterTextureTable['?'].texCoordY = texCoordY;
	asciiCharacterTextureTable['/'].textureNo = createTexture( loadCharacter('/', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['/'].width = fontImageWidth;	asciiCharacterTextureTable['/'].height = fontImageHeight;	asciiCharacterTextureTable['/'].texCoordX = texCoordX;	asciiCharacterTextureTable['/'].texCoordY = texCoordY;
	asciiCharacterTextureTable['_'].textureNo = createTexture( loadCharacter('_', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable['_'].width = fontImageWidth;	asciiCharacterTextureTable['_'].height = fontImageHeight;	asciiCharacterTextureTable['_'].texCoordX = texCoordX;	asciiCharacterTextureTable['_'].texCoordY = texCoordY;
	asciiCharacterTextureTable[' '].textureNo = createTexture( loadCharacter(' ', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );	asciiCharacterTextureTable[' '].width = fontImageWidth;	asciiCharacterTextureTable[' '].height = fontImageHeight;	asciiCharacterTextureTable[' '].texCoordX = texCoordX;	asciiCharacterTextureTable[' '].texCoordY = texCoordY;

	/* ?で埋める */
	for ( int i = 0; i < 127; i++ ){
		if ( asciiCharacterTextureTable[i].textureNo == 0 ){
			int width, height;
			asciiCharacterTextureTable[i].textureNo = createTexture( loadCharacter(' ', &fontImageWidth, &fontImageHeight, &texCoordX, &texCoordY), &width, &height );
			asciiCharacterTextureTable[i].width = fontImageWidth;
			asciiCharacterTextureTable[i].height = fontImageHeight;	
			asciiCharacterTextureTable[' '].texCoordX = texCoordX;
			asciiCharacterTextureTable[' '].texCoordY = texCoordY;
		}
	}

} // createASCIITextureTable

/* 文字を作る */
SDL_Surface *TextRenderer::loadCharacter(const char asciiCharacter, int *fontImageWidth, int *fontImageHeight, float *texCoordX, float *texCoordY){
	SDL_Surface *srcTextSurface;
	char tmpString[2];
	tmpString[0] = asciiCharacter;
	tmpString[1] = '\0';
	srcTextSurface = TTF_RenderText_Blended(font, tmpString, color);
	*fontImageWidth = srcTextSurface->w;
	*fontImageHeight = srcTextSurface->h;
	return convertImage( srcTextSurface, texCoordX, texCoordY );
} // loadCharacter

SDL_Surface *TextRenderer::loadText(const char *asciiText, SDL_Color color, float *texCoordX, float *texCoordY){
	SDL_Surface *srcTextSurface;
	srcTextSurface = TTF_RenderText_Blended(font, asciiText, color);
	return convertImage( srcTextSurface, texCoordX, texCoordY );
} // loadText

/* 読み込みと変換 */
SDL_Surface *TextRenderer::convertImage(SDL_Surface *srcSurface, float *texCoordW, float *texCoordH){


	int width, height;
	width = calcPowerOfTwo(srcSurface->w);
	height = calcPowerOfTwo(srcSurface->h);

	*texCoordW = (float)srcSurface->w / width;
	*texCoordH = (float)srcSurface->h / height;

	/* 空のGLテクスチャ用サーフェスの作成 */
	SDL_Surface *glSurface = SDL_CreateRGBSurface(
		SDL_SWSURFACE,
		width, height,
		32,
		#if SDL_BYTEORDER == SDL_LIL_ENDIAN						// OpenGL RGBA masks
					0x000000FF, 
					0x0000FF00,
					0x00FF0000,
					0xFF000000
		#else
					0xFF000000,
					0x00FF0000,
					0x0000FF00,
					0x000000FF
		#endif
	);

	/* アルファブレンディング属性の保存 */
	Uint32 savedFlags;				// 画像属性保存用変数
	Uint8  savedAlpha;				// アルファ属性保存用フラグ
	savedFlags = srcSurface->flags & ( SDL_SRCALPHA | SDL_RLEACCELOK );
	savedAlpha = srcSurface->format->alpha;
	if ( (savedFlags & SDL_SRCALPHA) == SDL_SRCALPHA ) {
		SDL_SetAlpha(srcSurface, 0, 0);
	}

	/* SDLサーフェスをGLテクスチャ用サーフェスへブリる */
	SDL_Rect region = {0, 0, srcSurface->w, srcSurface->h};
	SDL_BlitSurface(srcSurface, &region, glSurface, &region);
	SDL_FreeSurface(srcSurface);			// もういらない

	return glSurface;
} // convertImage

/* テクスチャ作成 */
GLuint TextRenderer::createTexture(SDL_Surface *srcImage, int *width, int *height){

	/* 新しいGLテクスチャ生成 */
	GLuint textureNo = 0;
	glGenTextures(1, &textureNo);
	glBindTexture(GL_TEXTURE_2D, textureNo);

	/* テクスチャパラメータの初期設定 */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	/* テクスチャの繰り返しの初期指定(繰り返し) */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	/* GLテクスチャに画像を割り当てる */
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, srcImage->w, srcImage->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, srcImage->pixels);

	if (width){
		*width = srcImage->w;
	}
	if (height){
		*height = srcImage->h;
	}

	SDL_FreeSurface(srcImage);

	return textureNo;

} // createTexture