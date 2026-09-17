.thumb
.global Mode2_StringGetterThunk_Impl
Mode2_StringGetterThunk_Impl:
  push {r4, lr}
  ldr r3, [r6, #16]
  ldrh r0, [r5]
  ldr r1, [sp, #16]
  mov r2, r7
  bl Mode2_CopyDisplayNicknameString_Impl
  pop {r4, pc}
