Texture Ground 
{
	filename=Ground.png	
}
                 
Texture Box
{
	filename=crate1.png
}

Texture Box1
{
	filename=crate2.png
}

Sprite Box
{
	texture=Box
	rect=0,0,128,128
}

Animation AnimBox1 : Box 
{
	texture=Box
	rect=0,0,128,128
}

Animation AnimBox2 : AnimBox1 
{
	texture=Box1	
}