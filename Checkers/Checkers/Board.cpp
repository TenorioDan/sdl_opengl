#include "Board.h"

Board::Board()
{

}

Board::Board(Texture& whiteTexture, Texture& greenTexture)
{
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (i & 1) {
				if (j & 1) {
					boardSpaces[i][j] = new BoardSpace(greenTexture, i * greenTexture.getWidth(), j * greenTexture.getHeight());
				}
				else {
					boardSpaces[i][j] = new BoardSpace(whiteTexture, i * whiteTexture.getWidth(), j * whiteTexture.getHeight());
				}
			}
			else {
				if (!(j & 1)) {
					boardSpaces[i][j] = new BoardSpace(greenTexture, i * greenTexture.getWidth(), j * greenTexture.getHeight());
				}
				else {
					boardSpaces[i][j] = new BoardSpace(whiteTexture, i * whiteTexture.getWidth(), j * whiteTexture.getHeight());
				}
			}
		}
	}
}


void Board::render(SDL_Renderer* renderer)
{
	for (int i = 0; i < boardRows; i++)
	{
		for (int j = 0; j < boardColumns; j++)
		{
			boardSpaces[i][j]->render(renderer);
		}
	}
}

int Board::getBoardSize()
{
	return boardColumns * boardRows;
}