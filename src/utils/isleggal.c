#include<stdio.h>
#include<regex.h>

int main(int argc, char** argv)
{
  if(IsLegalPage("www.runoob.com/regexp/regexp-syntax.html"))
    printf("该网页合法\n");
  else printf("该网页不合法！！！/n");
  return 0;
}

int IsLegalPage(char* s_tmp)
{
  // char* pattern="(https?)://.*(/.cn|/.com|/.htm|/.html|/.aspx?|/.jsp|/.php|/|/#)$";
  char* pattern="(https?)://.*(.cn|.com|.htm|.html|.aspx?|.jsp|.php|/|#)$";

  printf("pattern=%s\n",pattern);
  int z=0;
  regex_t reg;
  regmatch_t pm[1];
  z=regcomp(&reg,pattern,REG_EXTENDED|REG_ICASE);//编译
// REG_EXTENDED 以功能更加强大的扩展正则表达式的方式进行匹配。
// REG_ICASE 匹配字母时忽略大小写。
// REG_NOSUB 不用存储匹配后的结果。
// REG_NEWLINE 识别换行符，这样'$'就可以从行尾开始匹配，'^'就可以从行的开头开始匹配。
  z=regexec(&reg,s_tmp,1,pm,0);//匹配
  if(z!=0)
    return 0;
  else
    return 1;
  regfree(&reg);
}