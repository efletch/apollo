
public class CubeGenerator {

	public static void main(String[] args)
	{
		int count = 0;
		
		for(int x=1; x<20; x++)
		{
			for(int y =1; y<20; y++)
			{
				for(int z = 1; z<20; z++)
				{
					if(x==1 || x==19)
					{
						
							System.out.println(x+ " " + y + " " + z);
							count++;
						
					}
					
					else if(y==1 || y==19)
					{
						System.out.println(x+ " " + y + " " + z);
						count++;
					}
				}
			}
		}
		//System.out.println("Count " + count);
		
	}
	
	
}
