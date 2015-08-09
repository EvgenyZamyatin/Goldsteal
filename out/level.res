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

Texture Zombie 
{
	filename=zombie.jpg
}

Sprite Box
{
	texture=Box
	rect=0,0,128,128
	hotspot=0,128
}

Animation AnimBox1 
{
	texture=Box
	rect=0,0,128,128
	frames=1
	hotspot = 64,64
}

Animation AnimBox2
{
	texture=Box1
	rect=0,0,128,128
	fps=4.0
	frames=4
	hotspot = 64,64
}

Texture BlackSquare 
{
	filename=blacksquare.jpg
}

Sprite BlackSquare 
{
	texture=BlackSquare
	rect=0,0,16,16
	hotspot=0,16
}