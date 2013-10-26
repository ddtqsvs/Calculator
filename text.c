#include<reg52.h>
#include<math.h>
#define uchar unsigned char
#define uint unsigned int
unsigned char code leddp[]=
{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40};
//{0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xBF};//δ��С������������ʾ����,���һ���Ǹ���
uchar code leddot[]=
{0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,0x87,0xff,0xef};
//{0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};//��С�����������ʾ����
uchar code wey[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//��λ��ʾ��
uchar load[]={0,0,0,0,0,0,0,0};//��λװֵ����
long figure1,figure2,figure3,figure4;//�μ��������������
uchar keynum,err,fu,dotx,seat,decimal,count;
sbit p3_0=P3^0;
sbit p3_1=P3^1;
sbit p3_2=P3^2;
sbit p3_3=P3^3;
sbit p3_4=P3^4;
sbit p3_5=P3^5;
sbit p3_6=P3^6;
sbit p3_7=P3^7;

void display();
/*************************************
			��ʱ1ms��׼
**************************************/
void delay(uint i)
{
	uchar j;
	while(i--)
	for (j=0;j<230;j++);//��ʱ1�������
}


int Mult10(int n)
{
	int sum = 10;
	int m;
	if(n==0) return 1;
	for(m=1;m<n;m++) sum*=10;
	return sum;
}

//����ɨ�躯��
uchar keyscan()
{
	 uchar temp,scan1,scan2,num;
	 P1=0xf0;
	 scan1=P1;
	 if((scan1&0xf0)!=0xf0)
	 {
	 	delay(20);
		scan1=P1;
		if((scan1&0xf0)!=0xf0)
		{
			P1=0x0f;
			scan2=P1;
			temp=(scan1|scan2);
			switch(temp)
			{
				case 0xee:num=14;break;//����14
				case 0xde:num=13;break;//�˺�13
				case 0xbe:num=12;break;//����12
				case 0x7e:num=11;break;//�Ӻ�11
				case 0xed:num=15;break;//�Ⱥ�15
				case 0xdd:num=3;break;
				case 0xbd:num=6;break;
				case 0x7d:num=9;break;
				case 0xeb:num=16;break;//С����16
				case 0xdb:num=2;break;
				case 0xbb:num=5;break;
				case 0x7b:num=8;break;
				case 0xe7:num=0;break;
				case 0xd7:num=1;break;
				case 0xb7:num=4;break;
				case 0x77:num=7;break;
			}
			while(1)//�ȴ������ͷ�
			{
				P1=0xf0;
				if((P1&0xf0)==0xf0)
					break;
				else display();
			}
			return num;
		}
	 }
	if(p3_0==0||p3_1==0||p3_2==0||p3_3==0||p3_4==0||p3_5==0)
	{

	 	if(p3_0==0)
		{
			num=17;	//����AC
		}
		if(p3_1==0) 
		{
			num=18;	//��������
		}
		if(p3_2==0)
		{
			num=19;	//����MC
		}
		if(p3_3==0) 
		{
			num=20;	//����MR
		}
		if(p3_4==0)
		{
			num=21;	//����M+
		}
		if(p3_5==0) 
		{
			num=22;	//����M-
		}
		while(1)
		{
			if(p3_0==1&&p3_1==1&&p3_2==1&&p3_3==1&&p3_4==1&&p3_5==1)
			break;
			else display();
		}
		return num;
	}

	return 55;//��ʾ���ź�
}



void displayerror()
{
	P0=0x79;
	P2=0xef;
	delay(2);
	P0=0x77;
	P2=0xf2;
	delay(2);
	P0=0x3f;
	P2=0xfd;
	delay(2);
	P0=0;
}

//�������ʾ����

void display()
{
	if(err==1)
	{
		displayerror();
	}
	else
	{
	uchar i;		
	//��ʾ
	for(i=0;i<seat;i++)
	{
		if(i==dotx)
			P0=leddot[load[i]];//��dotxλ��С����
		else
			P0=leddp[load[i]];
		P2=wey[i];
		delay(2);
		P0=0;
	}
	}	 	  
}



//С����͸�����⺯��
void choose()
{
	uchar i;
	long figure;
	if(figure1>99999999||figure1<-9999999)
		err=1;	  //������ʾ��Χ
	if(figure1<0)
	{
		fu=1;		  //��ʾ����
	}
	else fu=0;
	if(decimal==0)
	{
		if(figure2==0) dotx=0;//��ʾ0λС��
		else if(figure2%10000==0) 
		{
			dotx=1;//��ʾ1λС��
			if(figure1>9999999) dotx=0;
		}
		else if(figure2%1000==0) 
		{
			dotx=2;//��ʾ2λС��
			if(figure1>999999) dotx=1;
		}
		else if(figure2%100==0) 
		{
			dotx=3;//��ʾ3λС��
			if(figure1>99999) dotx=2;
		}
		else if(figure2%10==0) 
		{
			dotx=4;//��ʾ4λС��
			if(figure1>9999) dotx=3;
		}
		else
		{
			dotx=5;//��ʾ5λС��
			if(figure1>999) dotx=4;
		}
	}
	else
	{dotx=count;}

	figure=fabs(figure1);
	//ȷ����ʾ��λ
	if(figure>9999999) 
		seat=8;
	else if(figure>999999)
		    seat=7+dotx+fu;
	else if(figure>99999)
		    seat=6+dotx+fu;
	else if(figure>9999)
		    seat=5+dotx+fu;
	else if(figure>999)
		    seat=4+dotx+fu;
	else if(figure>99)
		    seat=3+dotx+fu;
	else if(figure>9)
		    seat=2+dotx+fu;
	else
		seat=1+dotx+fu;
	if(seat>8) err=1;
	//����װ��������
	for(i=0;i<dotx;i++)
	{
		load[dotx-i-1]=figure2/Mult10(4-i)%10;//dotxλ����������ע��
	}
	for(i=dotx;i<seat-fu;i++)
	{
		load[i]=figure%10;//ȡ���λ
		figure=figure/10;//����
	}
	if(fu==1)
	{
		load[seat-1]=10;	//��ʾΪ����,���λ�踺��
	}

}




//������											   
void main()
{
	uchar /*decimal,count,*/need=1,mark=0,liandeng=0;
	double figure9;
	long figure01,figure02,figure0,mfigure1,mfigure2;
	figure1=figure2=figure3=figure4=keynum=decimal=count=mfigure1=mfigure2=0;	   //figure1��һ���������������� figure2��һ��������С������ figure3�ڶ����������������� figure4�ڶ���������С������
	P3=0xff;
	while(1)
	{	
		keynum=keyscan();
		if((keynum>=0)&&(keynum<=9))//�����������
		{
			if(liandeng==1)
			{
				figure1=figure2=figure3=figure4=keynum=decimal=count=0;
			}
			liandeng=0;
			if(decimal==0)//��������
				figure1=keynum+figure1*10;
			if(decimal==1)//С������
			{
				count++;
				switch(count)
				{
					case 1:figure2=keynum*10000;break;
					case 2:figure2=figure2+keynum*1000;break;
					case 3:figure2=figure2+keynum*100;break;
					case 4:figure2=figure2+keynum*10;break;
					case 5:figure2=figure2+keynum;break;
					case 6:err=1;
				}
			}
			need=1;
		}			  
		if((keynum>=11)&&(keynum<=14))
		{
			mark=keynum;//����
			figure3=figure1;
			figure4=figure2;
			figure1=0;
			figure2=0;
			decimal=0;
			liandeng=0;
			count=0;
		}
		if(keynum==15)//����Ⱥ�
		{
			if(liandeng==0)			  //�ж��Ƿ��һ������Ⱥ�
			{
				figure01=figure1;
				figure02=figure2;
				switch(mark)
				{
					case 11:					  //�ӷ�����
					{
						figure2=figure2+figure4;
						if(figure2>99999)
						{
							figure2=figure2%100000;
							figure1=figure1+figure3+1;
						}
						else
						{
							figure1=figure1+figure3;
						}
						break;
					}
					case 12:					  //��������
					{
						figure0=figure4-figure2;
						if(figure0<0)
						{
							figure2=figure4+100000-figure2;
							figure1=figure3-1-figure1;
						}
						else
						{
							figure2=figure4-figure2;
							figure1=figure3-figure1;
						}				
						break;
					}
					case 13:					  //�˷�����
					{
						figure9=((double)figure1+(double)figure2/100000)*((double)figure3+(double)figure4/100000);
						figure1=(long)figure9;
						figure2=(long)((figure9-figure1)*100000);
						break;
					}
					case 14:					 //��������
					{
						figure9=((double)figure3+(double)figure4/100000)/((double)figure1+(double)figure2/100000);
						figure1=(long)figure9;
						figure2=(long)((figure9-figure1)*100000);
						break;
					}	 
				}
			}
			else
			{
				switch(mark)
				{
					case 11:
					{
						figure2=figure2+figure02;
						if(figure2>99999)
						{
							figure2=figure2%100000;
							figure1=figure1+figure01+1;
						}
						else
						{
							figure1=figure1+figure01;
						}
						break;
					}
					case 12:
					{
						figure0=figure02-figure2;
						if(figure0<0)
						{
							figure2=figure02+100000-figure2;
							figure1=figure01-1-figure1;
						}
						else
						{
							figure2=figure02-figure2;
							figure1=figure01-figure1;
						}				
						break;
					}
					case 13:
					{
						figure9=((double)figure1+(double)figure2/100000)*((double)figure01+(double)figure02/100000);
						figure1=(long)figure9;
						figure2=(long)((figure9-figure1)*100000);
						break;
					}
					case 14:
					{
						figure9=((double)figure01+(double)figure02/100000)/((double)figure1+(double)figure2/100000);
						figure1=(long)figure9;
						figure2=(long)((figure9-figure1)*100000);
						break;
					}
				}
			}
			decimal=0;
			liandeng=1;
			need=1;
		}
		if(keynum==16)
		{
			decimal=1;//С��λ
		}
		if(keynum==18)//����
		{
			figure9=sqrt((double)figure1+(double)figure2/100000);
			figure1=(long)figure9;
			figure2=(long)((figure9-figure1)*100000);
			need=1;
		} 	
		if(keynum==17)//AC
		{
			figure1=figure2=figure3=figure4=keynum=decimal=count=0;
			liandeng=err=0;
			need=1;
		}
		if(keynum==19)//MC
		{
			mfigure1=0;
			mfigure2=0;
		}
		if(keynum==20)//MR
		{
			figure1=mfigure1;
			figure2=mfigure2;
			need=1;
		}
		if(keynum==21)//M+
		{
			mfigure1=figure1;
			mfigure2=figure2;
		}
		if(keynum==22)//M-
		{
			mfigure1=-figure1;
			mfigure2=figure2;
		}
		if(need==1)
		{
			choose();
		} 
		display();
		need=0;
	}
}







							