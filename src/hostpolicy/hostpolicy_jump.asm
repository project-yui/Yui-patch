
;
; created by AheadLibEx
; Author:i1tao
; Blog:https://www.cnblogs.com/0xc5
; Github:https://github.com/i1tao/AheadLibEx
;
;
; 把 .asm 文件添加到工程
; 右键单击文件-属性-常规
; 项类型:自定义生成工具
; 从生成中排除:否
;
; 然后复制下面命令填入
; 命令行: ml64 /Fo $(IntDir)%(fileName).obj /c /Cp %(fileName).asm
; 输出: $(IntDir)%(fileName).obj;%(Outputs)
; 链接对象: 是
;
; Add the .asm file into your project.
; Right click file -> Properties -> General
; Item Type : Custom Build Tool
; Excluded From Build : No
;
; In "Custom Build Tool" Properties
; Command Line :ml64 /Fo $(IntDir)%(fileName).obj /c /Cp %(fileName).asm
; Outputs:$(IntDir)%(fileName).obj;%(Outputs)
; Link Objects:Yes

.DATA
EXTERN pfnAheadLibEx_corehost_initialize:dq;
EXTERN pfnAheadLibEx_corehost_load:dq;
EXTERN pfnAheadLibEx_corehost_main:dq;
EXTERN pfnAheadLibEx_corehost_main_with_output_buffer:dq;
EXTERN pfnAheadLibEx_corehost_resolve_component_dependencies:dq;
EXTERN pfnAheadLibEx_corehost_set_error_writer:dq;
EXTERN pfnAheadLibEx_corehost_unload:dq;

.CODE
AheadLibEx_corehost_initialize PROC
	jmp pfnAheadLibEx_corehost_initialize
AheadLibEx_corehost_initialize ENDP

AheadLibEx_corehost_load PROC
	jmp pfnAheadLibEx_corehost_load
AheadLibEx_corehost_load ENDP

AheadLibEx_corehost_main PROC
	jmp pfnAheadLibEx_corehost_main
AheadLibEx_corehost_main ENDP

AheadLibEx_corehost_main_with_output_buffer PROC
	jmp pfnAheadLibEx_corehost_main_with_output_buffer
AheadLibEx_corehost_main_with_output_buffer ENDP

AheadLibEx_corehost_resolve_component_dependencies PROC
	jmp pfnAheadLibEx_corehost_resolve_component_dependencies
AheadLibEx_corehost_resolve_component_dependencies ENDP

AheadLibEx_corehost_set_error_writer PROC
	jmp pfnAheadLibEx_corehost_set_error_writer
AheadLibEx_corehost_set_error_writer ENDP

AheadLibEx_corehost_unload PROC
	jmp pfnAheadLibEx_corehost_unload
AheadLibEx_corehost_unload ENDP


END
