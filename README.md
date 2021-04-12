# BINARY
2020-1 동국대학교 컴퓨터 알고리즘과 실습 수업 -문자열 복원 알고리즘 설계 프로젝트 

## 개요
임의로 DNA sequence 문자열을 생성, 여러 알고리즘을 적용하여 문자열을 복원 및 성능 비교

## 개발 환경
* 개발 언어: C++
* 개발 IDE: visual studio
* 개발 OS: Window
* 실행 환경: Intel(R) Core(TM) i5-8265U CPU @ 1.60GHz 1.80GHz
* 기본 속도: 1.80GHz

## 사용된 알고리즘
* DENOVO
* BWT
* KMP
* BOYER MOORE
* TRIVIAL (기준) 
<br>
TRIVIAL 알고리즘을 기준으로 다른 알고리즘 성능과 시간 복잡도를 비교한다.

## 문제 조건
* 문제: 주어진 길이 L의 shortRead라는 string이 M개 있다. 이 shortRead를 이용하여 길이 N인 원본 문자열을 복원한다.
* N 길이: 최고 3,000,000,000
* M 크기: 대체적으로 2000만 ~ 2억
* L 길이: 대체적으로 32~100
* D: mismatch라는, shortRead와 해당 길이만큼의 원본 문자열 사이에 일치하지 않는 문자 개수

## 문제 풀이 단계
보다 상세한 설명은 ProjectReport/ BINARY_설계보고서.pdf 참조
1) 길이 N인 원본 문자열 생성
	* 중복 순열을 생성하는 함수를 재귀적으로 호출하여 생성한다. 
	이 함수는 ACGT 4개 문자만을 사용해 만들 수 있는, 모든 문자열을 생성한다. 이때, 이 문자열의 길이는 11로 지정한다. 
	* 랜덤 생성 함수<br>
	stdlib.h에서 제공하는 랜덤 함수를 사용한다. 
	* 이 원본 문자열은 단순히 길이 N인 문자열을 생성할 수도 있지만, 중복 순열을 생성하는 함수와 랜덤 생성 함수를 사용하여,
	일정한 규칙이 있는 문자열을 생성한다. 따라서, 실제 DNA sequence와 같은 형태를 가지는 문자열을 생성할 수 있다. <br>
	1번 문자열이라고 하자.
2) 원본 문자열 변경
	* 원본 문자열에서 일정한 간격을 두고 문자를 변경한다. <br> 
	이때 변경되는 문자는 DNA에서 SNP와 같은 역할이다. 사람의 DNA의 대부분은 비슷하지만, SNP가 다르다는 점을 활용하여 적용한 사항이다. 
	* 2번 문자열이라고 하자.
3) shortRead 생성 
	* 변경된 문자열을 잘라서 txt 파일에 저장한다. 
4) 4가지 알고리즘을 사용하여 성능 비교 
	* shortRead 문자열과 원본 문자열을 비교하여 변경된 문자열(2번 문자열)을 추측한다.
	* 추측하여 만들어진 문자열이 2번 문자열과 얼마나 비슷한지 비교하여 알고리즘의 성능을 측정한다. 
	* release 모드로 실행하여, 실행시간을 측정한다. 

<br>
보다 상세한 설명은 [프로젝트 소개](https://github.com/SeungMinSong2929/BINARY/tree/master/ProjectReport) 참고

## 결과 분석
* 성능 비교
![chart](https://user-images.githubusercontent.com/58732639/114364401-5193ee80-9bb4-11eb-9abd-820822f12050.png)
![graph](https://user-images.githubusercontent.com/58732639/114364804-bd765700-9bb4-11eb-80e8-ab115629ad21.png)

* TRIVIAL vs 각 알고리즘
	* trivial이 다른 알고리즘보다 더 빠른 것을 알 수 있다. 이는 trivial에서 check라는 변수를 이용하여 이미 check한 곳이면 다음으로 넘어가기
	때문이다. 원래 기본 trivial은 check라는 변수가 없기 때문에 기본 trivial과 비교한다면 다른 
	알고리즘들의 시간이 더 빠르게 나올 것이다.
* KMP vs BOYER MOORE
	* 전체적으로 BoyerMoore가 KMP보다 복원율이 높은 것을 볼 수 있다. 
	BoyerMoore가 KMP보다 복원율이 더 높은 이유는 KMP는 접두부를 이용하여 이동하는 반면 
	BoyerMoore는 패턴의 맨 뒤 문자만 비교하여 이동하기 때문이다. 접두부를 사용하게 되면 
	패턴내에 SNP이 있을 수 있기 때문에 제대로 된 접두부 위치를 찾지 못한다. 따라서 
	BoyerMoore는 맨 뒤 문자가 SNP가 아니면 되는 것에 비해 KMP는 접두부에 SNP가 없어야 
	하므로 복원율이 더 낮다. 
	* 또한 KMP가 복원율이 BoyerMoore보다 작은 이유 두 번째로는 check를 써서 한번 
	복원됐으면 더 이상 복원 못하게 하는 부분 때문이다. 복원된 곳이 잘못된 곳일 수 있다. 

## 함께한 팀원
* 김규리
* 송승민
* 최수정
* 이서연
