#include "Block.h"
Block::Block()
{
	memset(Block_Data, 0, 4096);
}
Block::~Block()
{

}
Block::Block(string blockname, int offset, File_Type filetype)
{
	Block_Name = blockname;
	Block_Offset = offset;
	Block_FileType = filetype;
}