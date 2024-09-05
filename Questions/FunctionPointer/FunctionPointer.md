<div dir="rtl">

# آموزش جامع Function Pointer و Function Pointer Array در C

Function Pointer و Function Pointer Array دو تا از مفاهیم قدرتمند و کاربردی در زبان C هستن که به شما اجازه می‌دن تا توابع رو مثل داده‌ها مدیریت کنید. این تکنیک‌ها می‌تونن به شما کمک کنن تا کدهای انعطاف‌پذیرتر و مرتب‌تری بنویسید، مخصوصاً وقتی که با سناریوهایی مثل مدیریت حالت‌های مختلف یا اجرای مجموعه‌ای از عملیات‌ها سروکار دارید.

در این راهنما، ما مرحله به مرحله با Function Pointer و Function Pointer Array آشنا می‌شیم و انواع مختلف اونا رو از لحاظ ورودی و خروجی بررسی می‌کنیم. هر سناریو رو اول بدون استفاده از این تکنیک‌ها پیاده‌سازی می‌کنیم و بعد بهبودش رو با استفاده از Function Pointer و Function Pointer Array نشون می‌دیم.

## 1. آشنایی با Function Pointer

### 1.1 تفاوت Function Pointer با بقیه Pointer‌ها

قبل از هر چیزی، بیاید تفاوت Function Pointer رو با سایر انواع Pointer‌ها بررسی کنیم:

- **Pointer به داده‌ها:** اشاره‌گرهایی مثل `int*` یا `char*` به یک آدرس حافظه که داده توش ذخیره شده اشاره می‌کنن.
- **Function Pointer:** این نوع اشاره‌گر به آدرس یک تابع در حافظه اشاره می‌کنه و بهتون اجازه می‌ده تا به طور غیرمستقیم اون تابع رو صدا بزنید، بدون اینکه لازم باشه اسم تابع رو تو کدتون بنویسید.

### 1.2 تعریف Function Pointer

Function Pointer ساختاری شبیه به تعریف توابع داره، با این تفاوت که به جای اسم تابع، از یک اشاره‌گر استفاده می‌کنیم. مثلاً فرض کنید یک تابع دارید که یک `int` ورودی می‌گیره و `void` برمی‌گردونه:

<div dir="ltr">

```c
void example_func(int a) {
    printf("Value: %d\n", a);
}
```
</div>

حالا اگه بخوای یک Function Pointer درست کنی که به این تابع اشاره کنه، به این شکل تعریفش می‌کنی:

<div dir="ltr">

```
void (*func_ptr)(int);
```
</div>

### توضیحات ساختار Function Pointer

- **نوع خروجی:** اولین چیزی که باید مشخص کنی، نوع خروجی تابعی هست که می‌خوای بهش اشاره کنی. مثلاً اگه تابع خروجی `int` داره، باید این قسمت رو `int` بنویسی.
- **نام Function Pointer:** بعدش باید اسم اشاره‌گر رو داخل پرانتز و همراه با یک `*` بنویسی. این `*` نشون می‌ده که این متغیر در واقع یک اشاره‌گر هست.
- **ورودی‌ها:** توی پرانتز بعد از اسم اشاره‌گر، نوع ورودی‌هایی که تابع می‌گیره رو مشخص می‌کنی. اگه تابع ورودی نداره، می‌تونی `void` بنویسی.

### مثال‌های دیگه:

- تابعی که `int` ورودی می‌گیره و `float` برمی‌گردونه:

<div dir="ltr">

```c
float (*func_ptr)(int);
```
</div>

- تابعی که دو int ورودی می‌گیره و int برمی‌گردونه:
  
<div dir="ltr">

```
int (*func_ptr)(int, int);
```

</div>

- تابعی که ورودی نداره و void برمی‌گردونه:

<div dir="ltr">

```
void (*func_ptr)(void);
```
</div>

## 2. انواع Function Pointer بر اساس ورودی و خروجی

### 2.1 Function Pointer با ورودی و بدون خروجی

این نوع Function Pointer به یک تابع اشاره می‌کنه که یک یا چند ورودی می‌گیره و چیزی برنمی‌گردونه. مثلاً:

<div dir="ltr">

```c
void (*func_ptr)(int, char);

void example_func(int a, char b) {
    printf("a: %d, b: %c\n", a, b);
}

int main() {
    func_ptr = example_func;
    func_ptr(5, 'A');
    return 0;
}
```
</div>

## 2.2 Function Pointer بدون ورودی و بدون خروجی
این نوع اشاره‌گر به تابعی اشاره می‌کنه که ورودی نداره و خروجی هم نداره:

<div dir="ltr">

``` c
void (*func_ptr)(void);

void example_func(void) {
    printf("Hello, World!\n");
}

int main() {
    func_ptr = example_func;
    func_ptr();
    return 0;
}
```

</div>

## 2.3 Function Pointer با ورودی و با خروجی
این نوع Function Pointer به تابعی اشاره می‌کنه که یک یا چند ورودی می‌گیره و مقداری رو برمی‌گردونه:

<div dir="ltr">

``` c
int (*func_ptr)(int, int);

int add(int a, int b) {
    return a + b;
}

int main() {
    func_ptr = add;
    int result = func_ptr(10, 20);
    printf("Result: %d\n", result);
    return 0;
}
```
</div>

## 2.4 Function Pointer بدون ورودی و با خروجی
در این حالت، تابعی که اشاره‌گر بهش اشاره می‌کنه، ورودی نداره ولی یک مقدار برمی‌گردونه:

<div dir="ltr">

```c
int (*func_ptr)(void);

int get_number(void) {
    return 42;
}

int main() {
    func_ptr = get_number;
    int result = func_ptr();
    printf("Result: %d\n", result);
    return 0;
}
```

</div>

## 3. کاربرد Function Pointer در برنامه‌نویسی

### سناریو 1: مدیریت حالت‌های مختلف با Function Pointer

فرض کن تو یک برنامه حالت‌های مختلفی داری، مثل حالت عادی، صرفه‌جویی در انرژی و پردازش داده. اول بیا این مسئله رو بدون استفاده از Function Pointer حل کنیم و بعد با Function Pointer بهبودش بدیم.

#### 3.1 پیاده‌سازی بدون Function Pointer

اینجا از `if` یا `switch` برای مدیریت حالت‌ها استفاده می‌کنیم.

<div dir="ltr">

```c
void low_power_mode() {
    printf("Entering Low Power Mode\n");
    // عملیات مربوط به حالت صرفه‌جویی در انرژی
}

void normal_mode() {
    printf("Entering Normal Mode\n");
    // عملیات مربوط به حالت عادی
}

void data_processing_mode() {
    printf("Entering Data Processing Mode\n");
    // عملیات مربوط به حالت پردازش داده
}

int main() {
    int mode = 1;  // فرض کنیم 1 یعنی حالت عادی

    if (mode == 0) {
        low_power_mode();
    } else if (mode == 1) {
        normal_mode();
    } else if (mode == 2) {
        data_processing_mode();
    }

    return 0;
}
```
</div>

#### مشکلات روش بدون Function Pointer

1. **کدهای شرطی پیچیده:** هر چی حالت‌های بیشتری داشته باشی، کدت پیچیده‌تر و مدیریت سخت‌تر می‌شه.
2. **انعطاف‌پذیری کمتر:** اضافه کردن حالت جدید یا تغییر ترتیب اجرا سخت‌تر می‌شه.
3. **نگهداری دشوار:** اگه بخوای تغییری تو یکی از حالت‌ها بدی، باید کل کدهای شرطی رو بررسی کنی.

#### 3.2 پیاده‌سازی با Function Pointer

حالا بیا ببینیم چطور با Function Pointer می‌تونیم این مسئله رو بهتر و انعطاف‌پذیرتر حل کنیم.

<div dir="ltr">

```c
typedef void (*StateFunction)(void);

void low_power_mode() {
    printf("Entering Low Power Mode\n");
}

void normal_mode() {
    printf("Entering Normal Mode\n");
}

void data_processing_mode() {
    printf("Entering Data Processing Mode\n");
}

int main() {
    StateFunction current_state;

    current_state = normal_mode;
    current_state();

    current_state = data_processing_mode;
    current_state();

    return 0;
}
```
</div>

### سناریو 2: ساخت منو با Function Pointer

فرض کن می‌خوای یک منو بسازی که کاربر با وارد کردن یک عدد، تابع مربوط به اون گزینه رو اجرا کنه. اول بدون Function Pointer این کار رو انجام می‌دیم و بعد با استفاده از Function Pointer کد رو بهبود می‌بخشیم.

#### 3.3 پیاده‌سازی بدون Function Pointer

اینجا از `switch` برای انتخاب گزینه منو و اجرای تابع مربوط به اون استفاده می‌کنیم.

<div dir="ltr">

```c
void option_1() {
    printf("You selected option 1\n");
}

void option_2() {
    printf("You selected option 2\n");
}

void option_3() {
    printf("You selected option 3\n");
}

int main() {
    int choice;
    printf("Enter your choice (1-3): ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            option_1();
            break;
        case 2:
            option_2();
            break;
        case 3:
            option_3();
            break;
        default:
            printf("Invalid choice\n");
            break;
    }

    return 0;
}
```

</div>

#### مشکلات روش بدون Function Pointer

1. **کد پیچیده و طولانی:** هر چی تعداد گزینه‌ها بیشتر بشه، کد طولانی‌تر و مدیریت سخت‌تر می‌شه.
2. **افزودن گزینه جدید:** اضافه کردن گزینه جدید نیازمند تغییرات زیادی در کد هست.

#### 3.4 پیاده‌سازی با Function Pointer

حالا بیا ببینیم چطور با Function Pointer می‌تونیم منو رو به شکل ساده‌تر و قابل توسعه‌تر پیاده‌سازی کنیم.

<div dir="ltr">

```c
void option_1() {
    printf("You selected option 1\n");
}

void option_2() {
    printf("You selected option 2\n");
}

void option_3() {
    printf("You selected option 3\n");
}

int main() {
    void (*menu[])(void) = {option_1, option_2, option_3};

    int choice;
    printf("Enter your choice (0-2): ");
    scanf("%d", &choice);

    if (choice >= 0 && choice <= 2) {
        menu[choice]();
    } else {
        printf("Invalid choice\n");
    }

    return 0;
}
```

</div>

### سناریو 3: انتقال توابع به عنوان پارامتر

فرض کن می‌خوای تابعی بنویسی که عملیات‌های مختلفی مثل جمع یا ضرب رو انجام بده و تابع مورد نظر رو به عنوان پارامتر به این تابع ارسال کنی. اول این مسئله رو بدون Function Pointer پیاده‌سازی می‌کنیم.

#### 3.5 پیاده‌سازی بدون Function Pointer

<div dir="ltr">

```c
int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

int main() {
    int x = 10, y = 20;

    int result_add = add(x, y);
    printf("Add: %d\n", result_add);

    int result_multiply = multiply(x, y);
    printf("Multiply: %d\n", result_multiply);

    return 0;
}
```
</div>

#### مشکلات روش بدون Function Pointer

1. **عدم انعطاف‌پذیری:** اگه بخوای عملیات جدیدی اضافه کنی، باید تابع جدیدی بنویسی و کد رو تغییر بدی.
2. **تکرار کد:** برای هر عملیات، بخش‌های زیادی از کد تکرار می‌شه.

#### 3.6 پیاده‌سازی با Function Pointer

<div dir="ltr">

```c
void execute_operation(int (*operation)(int, int), int x, int y) {
    int result = operation(x, y);
    printf("Result: %d\n", result);
}

int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

int main() {
    execute_operation(add, 10, 20);
    execute_operation(multiply, 10, 20);
    return 0;
}
```
</div>

### مزایای استفاده از Function Pointer

1. **کد مرتب‌تر و ساده‌تر:** نیازی به ساختارهای شرطی پیچیده نیست و کد مرتب‌تر و تمیزتر می‌شه.
2. **انعطاف‌پذیری بالا:** اضافه کردن یا تغییر توابع و حالت‌ها خیلی راحت‌تره.
3. **نگهداری و توسعه ساده‌تر:** هر وقت بخوای تابع جدیدی اضافه کنی یا تغییر بدی، فقط کافیه اون تابع رو به Function Pointer مرتبط کنی.


## استفاده از Function Pointer Array در C

Function Pointer Array (آرایه‌ای از اشاره‌گرهای تابع) یک ابزار قدرتمند دیگه در زبان C هست که بهت اجازه می‌ده تا مجموعه‌ای از توابع رو به صورت منظم و قابل دسترس نگهداری کنی. این روش به خصوص وقتی که با سناریوهایی سروکار داری که نیاز به اجرای توابع مختلف به صورت دینامیک یا بر اساس ورودی‌های کاربر دارن، خیلی مفیده.

### سناریو 1: مدیریت عملیات ریاضی

فرض کن یک ماشین‌حساب ساده داری که می‌خوای عملیات‌هایی مثل جمع، تفریق، ضرب و تقسیم رو مدیریت کنی. اول این مسئله رو بدون Function Pointer Array حل می‌کنیم.

#### 1.1 پیاده‌سازی بدون Function Pointer Array

<div dir="ltr">

```c
#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}

int divide(int a, int b) {
    if (b != 0) return a / b;
    return 0;
}

int main() {
    int a = 10, b = 5;
    char operation = '+';

    int result;

    if (operation == '+') {
        result = add(a, b);
    } else if (operation == '-') {
        result = subtract(a, b);
    } else if (operation == '*') {
        result = multiply(a, b);
    } else if (operation == '/') {
        result = divide(a, b);
    } else {
        printf("Invalid operation\n");
        return 1;
    }

    printf("Result: %d\n", result);

    return 0;
}
```
</div>

#### مشکلات روش بدون Function Pointer Array

1. **کد پیچیده و طولانی:** هر چی تعداد عملیات‌ها بیشتر بشه، کد طولانی‌تر و مدیریت اون سخت‌تر می‌شه.
2. **افزودن عملیات جدید:** اضافه کردن یک عملیات جدید نیازمند تغییرات گسترده توی کد هست.
3. **انعطاف‌پذیری کمتر:** مدیریت عملیات‌ها به روش دینامیک (مثلاً بر اساس ورودی‌های کاربر) خیلی سخت می‌شه.

#### 1.2 پیاده‌سازی با Function Pointer Array

<div dir="ltr">

```c
#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}

int divide(int a, int b) {
    if (b != 0) return a / b;
    return 0;
}

int main() {
    int (*operations[])(int, int) = {add, subtract, multiply, divide};
    char symbols[] = {'+', '-', '*', '/'};

    int a = 10, b = 5;
    char operation = '*';

    int result = 0;

    for (int i = 0; i < 4; i++) {
        if (operation == symbols[i]) {
            result = operations[i](a, b);
            break;
        }
    }

    printf("Result: %d\n", result);

    return 0;
}
```
</div>

### سناریو 2: منوی پویا برای یک برنامه نهفته

فرض کن داری یک سیستم نهفته طراحی می‌کنی که چند تا عملیات مختلف مثل کنترل LED، خواندن سنسور دما و ارسال داده از طریق سریال رو انجام می‌ده. می‌خوای یک منوی پویا بسازی که کاربر با وارد کردن یک عدد، عملیات مربوطه رو انجام بده.

#### 2.1 پیاده‌سازی بدون Function Pointer Array

<div dir="ltr">

```c
#include <stdio.h>

void control_led() {
    printf("LED is now ON\n");
}

void read_temperature() {
    printf("Temperature is 25°C\n");
}

void send_serial_data() {
    printf("Data sent over serial\n");
}

int main() {
    int choice = 2;

    switch (choice) {
        case 1:
            control_led();
            break;
        case 2:
            read_temperature();
            break;
        case 3:
            send_serial_data();
            break;
        default:
            printf("Invalid choice\n");
            break;
    }

    return 0;
}
```
</div>

#### مشکلات روش بدون Function Pointer Array

1. **کد پیچیده و طولانی:** اگه تعداد عملیات‌ها زیاد بشه، مدیریت و نگهداری این کد سخت می‌شه.
2. **افزودن عملیات جدید:** اضافه کردن یک عملیات جدید نیازمند تغییرات گسترده در کده.
3. **انعطاف‌پذیری کمتر:** امکان مدیریت عملیات به صورت پویا خیلی کم می‌شه.

#### 2.2 پیاده‌سازی با Function Pointer Array

<div dir="ltr">

```c
#include <stdio.h>

void control_led() {
    printf("LED is now ON\n");
}

void read_temperature() {
    printf("Temperature is 25°C\n");
}

void send_serial_data() {
    printf("Data sent over serial\n");
}

int main() {
    void (*menu[])(void) = {control_led, read_temperature, send_serial_data};

    int choice = 1;

    if (choice >= 0 && choice < 3) {
        menu[choice]();
    } else {
        printf("Invalid choice\n");
    }

    return 0;
}
```
</div>

### سناریو 3: اجرای یک سری عملیات به ترتیب

فرض کن یک سری عملیات مختلف داری که باید به ترتیب اجرا بشن. مثلاً باز کردن یک فایل، خوندن اطلاعات، پردازش اطلاعات و بستن فایل. این مسئله رو اول بدون Function Pointer Array و بعد با اون حل می‌کنیم.

#### 3.1 پیاده‌سازی بدون Function Pointer Array

<div dir="ltr">

```c
#include <stdio.h>

void open_file() {
    printf("File opened\n");
}

void read_data() {
    printf("Data read\n");
}

void process_data() {
    printf("Data processed\n");
}

void close_file() {
    printf("File closed\n");
}

int main() {
    open_file();
    read_data();
    process_data();
    close_file();

    return 0;
}
```
</div>

#### مشکلات روش بدون Function Pointer Array

1. **کد تکراری و طولانی:** برای هر عملیات باید تابع مربوطه رو به صورت دستی صدا بزنی.
2. **انعطاف‌پذیری کمتر:** امکان تغییر ترتیب عملیات یا اضافه کردن عملیات جدید به سادگی وجود نداره.

#### 3.2 پیاده‌سازی با Function Pointer Array

<div dir="ltr">

```c
#include <stdio.h>

void open_file() {
    printf("File opened\n");
}

void read_data() {
    printf("Data read\n");
}

void process_data() {
    printf("Data processed\n");
}

void close_file() {
    printf("File closed\n");
}

int main() {
    void (*operations[])(void) = {open_file, read_data, process_data, close_file};

    for (int i = 0; i < 4; i++) {
        operations[i]();
    }

    return 0;
}
```

</div>

### نتیجه‌گیری

Function Pointer Array یک ابزار قدرتمند برای مدیریت مجموعه‌ای از توابع به شکل منظم و پویاست. با استفاده از این تکنیک، می‌تونی کدهای خودت رو ساده‌تر، انعطاف‌پذیرتر و قابل گسترش‌تر کنی. این روش به خصوص توی سیستم‌های نهفته و برنامه‌های کاربردی که نیاز به مدیریت دینامیک توابع دارن، خیلی به درد می‌خوره.

---

### تمرین‌ها

| شماره | سوال                                                                                                                                                                                              | بارم |
| ----- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---- |
| 1     | یک برنامه بنویس که از Function Pointer Array برای مدیریت چند تابع ریاضی مثل جمع، تفریق، ضرب و تقسیم استفاده کنه.                                                                                  | 3    |
| 2     | یک برنامه منوی پویا بنویس که از Function Pointer Array برای مدیریت چند عملیات مختلف در سیستم نهفته استفاده کنه. مثلاً کارهایی مثل کنترل LED، خواندن سنسور دما، و ارسال داده‌های سریال رو انجام بده. | 4    |
| 3     | یک برنامه بنویس که یک سری عملیات مختلف مثل باز کردن فایل، خوندن داده، پردازش داده و بستن فایل رو به ترتیب با استفاده از Function Pointer Array اجرا کنه.                                          | 4    |
| 4     | یک برنامه بنویس که از Function Pointer Array برای مدیریت و اجرای توابع بر اساس ورودی کاربر استفاده کنه. مثلاً کاربر می‌تونه عددی وارد کنه و برنامه تابع مرتبط با اون عدد رو اجرا کنه.               | 5    |

---

این سناریوها و تمرین‌ها بهت کمک می‌کنن تا Function Pointer Array رو به طور کامل یاد بگیری و ازش تو پروژه‌هات استفاده کنی. اگه سوالی داشتی یا نیاز به راهنمایی بیشتر داشتی، تو قسمت issue ها میتونی باهام در ارتباط باشی!



</div>