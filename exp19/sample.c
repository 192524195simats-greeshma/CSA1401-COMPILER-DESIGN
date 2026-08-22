#include &lt;stdio.h&gt;
int main()
{
int number1, number2, sum;
printf(&quot;Enter two integers: &quot;);
scanf(&quot;%d %d&quot;, &amp;number1, &amp;number2);
sum = number1 + number2;
printf(&quot;%d + %d = %d&quot;, number1, number2, sum);
return 0;
}