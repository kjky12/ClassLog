# ClassLog
싱글톤 패턴을 이용한 로그 클래스를 제공합니다.

2020-01-14
기존 생성했던 로그 클래스 기능 업데이트
- 일자에 따라 자동 디렉토리 생성함수 추가
- 자동 생성 루트 디렉토리 생성함수 추가

#하단 URL을 참조하세요
URL : https://kjk92.tistory.com/52

////////////////////////////////////////////////////////////////////////////////////////////////
// 사용 방법 예시

	#include "Log.h"
	void Temp()
	{
  		CLog *pLog = NULL;
		pLog = CLog::Instance();

		pLog->SetAutoDirectory(TRUE);
		pLog->SetAutoDirectoryName(gf_GetModulePath() + L"Restartlog");
    
  		pLog->Log(L"로그생성!");
	}
