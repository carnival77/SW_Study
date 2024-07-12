- [Autowiring](#autowiring)
	- [@Autowired](#autowired)
	- [@Autowired 구현 방법](#autowired-구현-방법)
		- [생성자 주입](#생성자-주입)
		- [수정자 주입](#수정자-주입)
		- [필드 주입](#필드-주입)
		- [생성자 주입을 선택해야하는 이유](#생성자-주입을-선택해야하는-이유)
		- [생성자 주입 정리](#생성자-주입-정리)
		- [@Autowired 대상 찾기](#autowired-대상-찾기)
			- [@Primary](#primary)
			- [@Qualifier](#qualifier)
	- [Reference](#reference)

---

# **Autowiring**

스프링 컨테이너에 빈들을 모두 등록한 후에, 의존성 주입 단계가 이루어질 때, @Autowired 어노테이션이 부여된 해당 변수 및 메서드에 스프링이 관리하는 Bean을 자동으로 매핑해주는 개념이다. 즉, 메서드가 실행되며 필요한 인스턴스를 주입하거나 해당 변수에 자동으로 빈을 매핑한다.

## **@Autowired**

* 스프링 컨테이너에 등록한 빈에게 의존 관계 주입이 필요할 때, DI(의존성 주입)을 도와주는 어노테이션
* 변수, setter 메서드, 생성자, 일반 메서드에 적용 가능
* 의존 객체 주입 시 주로 Type 이용
  * @Autowired 가 Type으로 찾아서 주입하므로 동일한 Bean 타입의 객체가 여러 개 있을 경우, Bean을 찾기 위해 @Qualifier 어노테이션을 같이 사용해야 합니다. 

## @Autowired 구현 방법

* @Autowired는 총 3가지 방법으로 실현이 가능한데, **생성자, 수정자(setter), 필드**를 사용할 수 있다.

### 생성자 주입

- 방법 : **Constructor 생성자를 통해 의존 관계를 주입**하는 방법이다.
- 객체가 생성될 때 딱 한 번 호출되는 것이 보장된다 → 의존관계가 변하지 않는 경우, 필수 의존관계에 사용
- 의존 관계에 있는 객체들을 **final**로 선언할 수 있다는 장점 → 생성자에서 무조건 설정해주어야 함 → 누락 발생 X
- 생성자가 하나일 경우 @Autowired를 생략할 수 있다.


예를 들어 OrderService라는 클래스가 MemberRepository와 DiscountPolicy 클래스에 의존한다고 가정하자.

```java
@Component
public class OrderServiceImpl implements OrderService {
	private final MemberRepository memberRepository;
	private final DiscountPolicy discountPolicy;
	@Autowired
	public OrderServiceImpl(MemberRepository memberRepository, DiscountPolicy discountPolicy) {
            this.memberRepository = memberRepository;
            this.discountPolicy = discountPolicy;
	}
}
```

의존해야 하는 객체를 우선 private final로 선언한다.

그리고 생성자를 통해 해당 객체들을 주입받도록 하고, 생성자의 위에 @Autowired 어노테이션을 추가한다.

### **수정자 주입**

- 방법 : **setter를 생성하고, 그 위에 @Autowired**를 적는다.
- 스프링 빈을 모두 등록한 후에 @Autowired가 붙은 수정자를 모두 찾아서 의존관계를 주입한다.
- "**선택적**"이고, "**변화 가능**"한 의존 관계에 사용한다.

```java
@Component
public class OrderServiceImpl implements OrderService {
	private MemberRepository memberRepository;
	private DiscountPolicy discountPolicy;
	@Autowired
	public void setMemberRepository(MemberRepository memberRepository) {
	    this.memberRepository = memberRepository;
	}
	@Autowired
	public void setDiscountPolicy(DiscountPolicy discountPolicy) {
	    this.discountPolicy = discountPolicy;
	}
}
```

자바 빈 프로퍼티 규약에 따라 해당 변수에 대한 Setter 메서드를 생성하고, 해당 setter에 @Autowired를 붙여서 사용한다.

### 필드 주입
- 제일 간단한 방법이다.
- **변수에 @Autowired**를 붙여서 사용한다.
- 하지만 **단점이 너무 많다**. 어플리케이션과 관련이 없는 테스트 코드에서만 한정적으로 사용하자.

```java
@Component
public class OrderServiceImpl implements OrderService {
	@Autowired
	private MemberRepository memberRepository;
	@Autowired
	private DiscountPolicy discountPolicy;
}
```

### 생성자 주입을 선택해야하는 이유
- 불변
  - 대부분의 의존 관계는 어플리케이션 종료까지 변경될 일이 없다 = 불변
  - 만일 수정자 주입을 사용한다면 setter를 public으로 설정해야 한다. → 실수로 변경 가능 = 좋은 방법이 아님
  - 만일 생성자 주입을 사용한다면, 생성자 호출 시점에 1번 호출 → **불변하게 설계 가능**
- 누락
  - 프레임워크 없이 순수한 자바코드를 통해 단위 테스트하는 일이 많다.
  - 수정자 주입을 사용하면 임의의 관련 객체를 만들어야한다. → 의존관계가 한 눈에 보이지 않아 누락 발생 가능
  - 생성자 주입을 사용하면 누락을 막을 수 있다.
- **final 키워드**의 사용
  - 생성자 주입 사용 시 final 키워드 사용 가능 → 생성자를 통해서만 설정 가능
  - 최고의 장점 → 컴파일 오류를 통해 “**누락**”을 놓치지 않을 수 있다.


### 생성자 주입 정리
- 생성자 주입 → 순수한 자바 언어의 특징을 잘 살리는 방법이다
- “기본적으로 생성자 주입”을 사용하고, 필수값이 아닌 경우 setter 주입 방식을 옵션으로 부여하자. ⇒ 생성자 주입과 setter 주입을 동시에 사용할 수 있다.
- 필드 주입은 사용하지 말자!

### @Autowired 대상 찾기
@Autowired를 통한 의존 객체 주입 시 주로 Type을 이용한다. 동일한 Bean 타입의 객체가 여러 개 있을 경우에 해결 방법은?

```java
//interface
public interface CustRepository {
}

@Repository
public class MyCustRepository implements CustRepository {
}

@Repository
public class MesungCustRepository implements CustRepository {
}

//CustService
public class CustService {

@Autowired
CustRepository custRepository;
}
```

- 위 같이 같은 타입의 객체를 두 개 이상 정의하고 의존성 주입을 받을 시 주입을 받지 못한다.

- Spring은 어떤 의존성을 주입해야 하는 지 모르는 것이다.

- 이 때의 해결방법은? @Primary, @Qualifier, 해당 타입의 빈 모두 받기

#### @Primary

```java
@Repository
public class MyCustRepository implements CustRepository {
}

@Repository @Primary
public class MesungCustRepository implements CustRepository {
}
```

- @Primary를 사용할 시 같은 타입의 객체가 여러 개 있을 경우 해당 어노테이션이 붙은 객체를 의존 받겠다 라는 의미가 된다.

#### @Qualifier

```java
//CustService
public class CustService {
@Autowired @Qualifier("mesungCustRepository")
CustRepository custRepository;
}
```

- @Qualifier를 사용할 시 mesungCustRepository를 의존 받을 수 있는 것이다.

  

## Reference

> - [[스프링 핵심] @Autowired](https://it-mesung.tistory.com/29)
> - [[Spring] @Autowired를 통한 의존 관계 주입](https://m42-orion.tistory.com/100)