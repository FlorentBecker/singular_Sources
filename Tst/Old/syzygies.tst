test(11);
ring r=31991,(x,y,z,w),(c,dp);
ideal regsec1=x2+x2y+x3,y2+y2z+y3,z2+z2w+z3,w2+w2x+w3;
ideal regsec2=x2*(1+y+yz+yzw),y2*(1+z+zw+zwx),z2*(1+w+wx+wxy);
ideal regsec3=x3+yz4+zw4,y3+z4w+w4x,z3+wx4+xy4,w3+x4y+y4z;
module syz1=syz(regsec1);
matrix m1[1][1]=matrix(regsec1);
matrix s1[1][1]=matrix(syz1);
m1*s1;
module syz2=syz(regsec2);
m1=matrix(regsec2);
s1=matrix(syz2);
m1*s1;
module syz3=syz(regsec3);
m1=matrix(regsec3);
s1=matrix(syz3);
m1*s1;
syz1;
syz2;
syz3;
kill r;
ring r1=31991,(x,y,z,w),(c,ls);
test(-11,17);
ideal regsec4=x2+x2y+x3,y2+y2z+y3,z2+z2w+z3,w2+w2x+w3;
ideal regsec5=x2*(1+y+yz+yzw),y2*(1+z+zw+zwx),z2*(1+w+wx+wxy);
ideal regsec6=x3+yz4+zw4,y3+z4w+w4x,z3+wx4+xy4,w3+x4y+y4z;
module syz4=syz(regsec4);
matrix m1[1][1]=matrix(regsec4);
matrix s1[1][1]=matrix(syz4);
m1*s1;
module syz5=syz(regsec5);
m1=matrix(regsec5);
s1=matrix(syz5);
m1*s1;
module syz6=syz(regsec6);
m1=matrix(regsec6);
s1=matrix(syz6);
m1*s1;
syz4;
syz5;
syz6;
kill r1;
ring r2=0,(t,x,y,z,w),(c,dp);
test(-17);
option(intStrategy);
ideal regsec7=x2+x2y+x3,y2+y2z+y3,z2+z2w+z3,w2+w2x+w3;
ideal regsec8=x2*(1+y+yz+yzw),y2*(1+z+zw+zwx),z2*(1+w+wx+wxy);
ideal regsec9=x3+yz4+zw4,y3+z4w+w4x,z3+wx4+xy4,w3+x4y+y4z;
module syz7=syz(homog(regsec7,t));
matrix m1[1][1]=matrix(homog(regsec7,t));
matrix s1[1][1]=matrix(syz7);
m1*s1;
module syz8=syz(homog(regsec8,t));
m1=matrix(homog(regsec8,t));
s1=matrix(syz8);
m1*s1;
module syz9=syz(homog(regsec9,t));
m1=matrix(homog(regsec9,t));
s1=matrix(syz9);
m1*s1;
syz7;
syz8;
syz9;
LIB "tst.lib";tst_status(1);$

