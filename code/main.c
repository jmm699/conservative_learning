int main(int argc,char* argv[])
{
// command line inputs here

if(!getnet(netfile))
	return 1;
	
if(!opendata(trainfile,testfile))
	return 1;

setup();

// train & test

closedata();

return 0;
}