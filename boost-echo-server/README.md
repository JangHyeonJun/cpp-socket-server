# boost-echo-server

🎯 **Boost.Asio 기반 C++20 Echo Server**

이 프로젝트는 `Boost.Asio`와 C++20의 `co_await` 코루틴을 사용하여 만든 간단한 TCP 에코 서버입니다. 실습 목적이며, 이후 확장 가능한 구조로 발전시킬 계획입니다.

---

## 🔧 요구사항

- Windows 11 64bit
- Visual Studio 2022
- C++20 지원
- Boost (vcpkg 사용 권장)

```bash
# vcpkg 설치 후
vcpkg install boost-asio
