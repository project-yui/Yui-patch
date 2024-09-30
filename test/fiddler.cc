#include <cstdio>
#include <dlfcn.h>
typedef void (*cosf_method_type)();
int main(void)
{
    printf("fiddler test start....\n");
    void * libm_handle = NULL;
    void (*cosf_method)();
    char *errorInfo;
    float result;
    
    // dlopen 函数还会自动解析共享库中的依赖项。这样，如果您打开了一个依赖于其他共享库的对象，它就会自动加载它们。
    // 函数返回一个句柄，该句柄用于后续的 API 调用
    libm_handle = dlopen("../libfiddler.so", RTLD_LAZY );
    // 如果返回 NULL 句柄，表示无法找到对象文件，过程结束。否则的话，将会得到对象的一个句柄，可以进一步询问对象
    if (!libm_handle){
        // 如果返回 NULL 句柄,通过dlerror方法可以取得无法访问对象的原因
        printf("Open Error:%s.\n",dlerror());
        return 0;
    }
 
    // 使用 dlsym 函数，尝试解析新打开的对象文件中的符号。您将会得到一个有效的指向该符号的指针，或者是得到一个 NULL 并返回一个错误
    cosf_method = (cosf_method_type)dlsym(libm_handle, "initialize");
    errorInfo = dlerror();// 调用dlerror方法，返回错误信息的同时，内存中的错误信息被清空
    if (errorInfo != NULL){
        printf("Dlsym Error:%s.\n",errorInfo);
        return 0;
    }
 
    // 执行“cosf”方法
    (*cosf_method)();
    printf("result = %f.\n",result);
    
    // 调用 ELF 对象中的目标函数后，通过调用 dlclose 来关闭对它的访问
    dlclose(libm_handle);

    return 0;
}