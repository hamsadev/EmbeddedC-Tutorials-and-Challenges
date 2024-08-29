
<div dir="rtl">

# آموزش Alignment و Padding در C: از پایه تا پیشرفته با سناریوهای کاربردی

در زبان C، وقتی از `struct` استفاده می‌کنیم، موضوعاتی مثل Alignment (تراز کردن) و Padding (پُر کردن) اهمیت زیادی پیدا می‌کند. این مفاهیم تعیین می‌کنند که داده‌ها چطور در حافظه ذخیره می‌شوند و چطور به آن‌ها دسترسی پیدا می‌کنیم. در این مقاله، با استفاده از سناریوهای کاربردی، مفاهیم Alignment و Padding را به شکل کامل بررسی می‌کنیم. همچنین با استفاده از شکل‌هایی که با کاراکترهای متنی ساخته می‌شوند، سعی می‌کنیم این مفاهیم را به ساده‌ترین شکل ممکن توضیح دهیم.

## 1. مفهوم Alignment

در زبان C، داده‌ها باید به شکلی در حافظه ذخیره شوند که دسترسی به آن‌ها سریع و بهینه باشد. Alignment به این معناست که هر داده باید در آدرس حافظه‌ای ذخیره شود که با اندازه خودش هماهنگ باشد. مثلاً، یک داده `int` که معمولاً 4 بایت است، بهتر است در آدرسی ذخیره شود که مضربی از 4 باشد. این کار باعث می‌شود پردازنده بتواند داده را به شکل بهینه‌تری بخواند.

### 1.1 علت وجود Alignment

علت اصلی وجود Alignment به معماری پردازنده و سرعت دسترسی به داده‌ها برمی‌گردد. پردازنده‌ها معمولاً داده‌ها را در واحدهای مشخص (مثل 4 بایت یا 8 بایت) می‌خوانند. اگر داده‌ها در آدرسی ذخیره شوند که با این واحدها هماهنگ نباشد (مثلاً یک داده 4 بایتی در آدرسی که مضربی از 4 نیست)، پردازنده باید داده را به صورت چند تکه بخواند و دوباره آن را سرهم کند که این کار باعث کاهش سرعت می‌شود.

### سناریو 1: درک Alignment با یک مثال ساده

فرض کن یک `struct` داری که شامل دو متغیر است: یکی `char` و یکی `int`. در حالت عادی، انتظار داری که `struct` به اندازه جمع اندازه‌های این دو متغیر باشد، ولی به خاطر Alignment این اتفاق نمی‌افتد.

<div dir="ltr">

```c
#include <stdio.h>

typedef struct {
    char c;
    int i;
} MyStruct;

int main() {
    printf("Size of MyStruct: %lu\n", sizeof(MyStruct));
    return 0;
}
```
</div>


#### توضیح

در این مثال، `char` یک بایت و `int` چهار بایت فضا می‌گیرد. ولی وقتی سایز `struct` را چک می‌کنی، می‌بینی که اندازه آن به جای 5 بایت، 8 بایت است. چرا؟ به خاطر Alignment.

#### تصویرسازی Alignment

حالا ببینیم این `struct` چطور در حافظه ذخیره می‌شود:
<div dir="ltr">

```
Memory Layout: | c (1 byte) | Padding (3 bytes) | i (4 bytes) |
```
</div>

اینجا به این دلیل که `int` باید در آدرسی ذخیره شود که مضربی از 4 باشد، سه بایت بعد از `char` برای پُر کردن اضافه می‌شوند.

## 2. مفهوم Padding

Padding به معنی اضافه کردن بایت‌های خالی (ناخواسته) به `struct` است تا داده‌ها به درستی تراز شوند. این بایت‌های خالی به طور خودکار توسط کامپایلر اضافه می‌شوند تا مطمئن شود که هر فیلد در یک آدرس بهینه در حافظه قرار دارد.

### سناریو 2: درک Padding با تغییر ترتیب متغیرها

حالا بیایم ترتیب متغیرهای `struct` را تغییر دهیم و ببینیم چه اتفاقی می‌افتد. اگر `int` را اول و `char` را دوم قرار دهیم، Padding چطور تغییر می‌کند؟

<div dir="ltr">

```c
#include <stdio.h>

typedef struct {
    int i;
    char c;
} MyStruct;

int main() {
    printf("Size of MyStruct: %lu\n", sizeof(MyStruct));
    return 0;
}
```
</div>


#### توضیح

این بار، `int` اول قرار دارد و `char` بعد از آن می‌آید. چون `int` به درستی تراز شده، هیچ Padding قبل از آن اضافه نمی‌شود. اما بعد از `char`، 3 بایت Padding اضافه می‌شود تا ساختار به درستی تراز شود.

#### تصویرسازی Padding
<div dir="ltr">

```
Memory Layout: | i (4 bytes) | c (1 byte) | Padding (3 bytes) |
```
</div>

## 3. بهینه‌سازی Alignment و Padding

گاهی اوقات می‌توان با تغییر ترتیب فیلدها در `struct`، فضای حافظه را بهینه‌تر کرد و از Padding غیرضروری جلوگیری کرد.

### سناریو 3: بهینه‌سازی فضای حافظه با تغییر ترتیب فیلدها

فرض کن یک `struct` داری که شامل یک `char`، یک `int` و یک `double` است. اگر این فیلدها را به ترتیب زیر بچینی، چه اتفاقی می‌افتد؟

<div dir="ltr">

```c
#include <stdio.h>

typedef struct {
    char c;
    int i;
    double d;
} MyStruct;

int main() {
    printf("Size of MyStruct: %lu\n", sizeof(MyStruct));
    return 0;
}
```
</div>


#### توضیح

در این حالت، `char` و `int` هر دو نیاز به Padding دارند. اما اگر ترتیب فیلدها را تغییر دهیم تا ابتدا بزرگ‌ترین فیلد (`double`) قرار بگیرد، می‌توانیم از Padding جلوگیری کنیم.

#### بهینه‌سازی

حالا ببینیم چطور با تغییر ترتیب فیلدها می‌توانیم فضای حافظه را بهینه کنیم:

<div dir="ltr">

```c
#include <stdio.h>

typedef struct {
    double d;
    int i;
    char c;
} MyStruct;

int main() {
    printf("Size of MyStruct: %lu\n", sizeof(MyStruct));
    return 0;
}
```
</div>


#### تصویرسازی بهینه‌سازی
<div dir="ltr">

```
Memory Layout (بهینه‌سازی شده): | d (8 bytes) | i (4 bytes) | c (1 byte) | Padding (3 bytes) |

```
</div>

در این حالت، فضای Padding کمتر و حافظه بهینه‌تر شده است.


## 4. استفاده از #pragma و __attribute__ برای کنترل Alignment و Padding

در بعضی مواقع، نیاز داریم که کنترل بیشتری روی Alignment و Padding داشته باشیم. در چنین شرایطی می‌توانیم از `#pragma pack` یا `__attribute__ ((packed))` استفاده کنیم.

### 4.1 استفاده از `#pragma pack`

`#pragma pack` به ما اجازه می‌دهد که اندازه Alignment را تنظیم کنیم و از Padding اضافی جلوگیری کنیم. به عنوان مثال، اگر بخواهیم هیچ Padding بین فیلدهای یک `struct` وجود نداشته باشد، می‌توانیم از `#pragma pack(1)` استفاده کنیم:

<div dir="ltr">

```c
#include <stdio.h>

#pragma pack(1)
typedef struct {
    char c;
    int i;
    double d;
} MyStruct;

int main() {
    printf("Size of MyStruct: %lu\n", sizeof(MyStruct));
    return 0;
}
```
</div>


#### توضیح

با استفاده از `#pragma pack(1)`، کامپایلر را مجبور می‌کنیم که هیچ Padding بین فیلدها اضافه نکند. این باعث می‌شود که سایز `struct` دقیقاً برابر با جمع اندازه‌های فیلدها باشد.

### 4.2 استفاده از `__attribute__ ((packed))`

`__attribute__ ((packed))` نیز به ما اجازه می‌دهد که یک `struct` را بدون Padding تعریف کنیم:

<div dir="ltr">

```c
#include <stdio.h>

typedef struct {
    char c;
    int i;
    double d;
} __attribute__ ((packed)) MyStruct;

int main() {
    printf("Size of MyStruct: %lu\n", sizeof(MyStruct));
    return 0;
}
```
</div>


#### توضیح

در این مثال، با استفاده از `__attribute__ ((packed))`، از اضافه شدن Padding به `struct` جلوگیری می‌کنیم.

## 5. ترکیب Alignment و Padding با Union

گاهی اوقات از `union` استفاده می‌کنیم تا داده‌های مختلف را در یک فضای حافظه نگه داریم. وقتی `union` با `struct` ترکیب شود، Alignment و Padding همچنان اهمیت دارند.

### 5.1 سناریو: درک ترکیب Alignment و Padding با Union

فرض کن یک `union` داری که شامل یک `int` و یک `char` است. اگر این `union` را داخل یک `struct` قرار دهیم که خودش هم شامل یک `double` است، چه اتفاقی می‌افتد؟

<div dir="ltr">

```c
#include <stdio.h>

typedef union {
    int i;
    char c;
} MyUnion;

typedef struct {
    double d;
    MyUnion u;
} MyStruct;

int main() {
    printf("Size of MyStruct: %lu\n", sizeof(MyStruct));
    return 0;
}
```
</div>


#### توضیح

در این حالت، `double` به درستی تراز شده است. اما چون `union` شامل `int` است، `union` هم باید به درستی تراز شود. این به معنی اضافه شدن Padding قبل از `union` است.

#### تصویرسازی ترکیب
<div dir="ltr">

```
Memory Layout: | d (8 bytes) | Padding (4 bytes) | u (4 bytes) |
```
</div>

## 6. ترکیب Alignment و Padding با آرایه‌ها

وقتی `struct` ها را به صورت آرایه تعریف می‌کنیم، Padding و Alignment همچنان اهمیت دارند.

### 6.1 سناریو: درک ترکیب Alignment و Padding با آرایه‌ها

فرض کن یک `struct` داری که شامل یک `char` و یک `int` است. حالا این `struct` را به صورت آرایه تعریف می‌کنیم:

<div dir="ltr">

```c
#include <stdio.h>

typedef struct {
    char c;
    int i;
} MyStruct;

int main() {
    MyStruct arr[3];
    printf("Size of MyStruct: %lu\n", sizeof(MyStruct));
    printf("Size of array: %lu\n", sizeof(arr));
    return 0;
}
```
</div>


#### توضیح

هر عنصر از آرایه باید به درستی تراز شود، بنابراین Padding بعد از هر عنصر اضافه می‌شود تا عنصر بعدی نیز به درستی تراز شود.

#### تصویرسازی ترکیب
<div dir="ltr">

```
Memory Layout: | c (1 byte) | Padding (3 bytes) | i (4 bytes) | <- Element 1 | c (1 byte) | Padding (3 bytes) | i (4 bytes) | <- Element 2 | c (1 byte) | Padding (3 bytes) | i (4 bytes) | <- Element 3
```
</div>

## 7. جمع‌بندی

در این مقاله، با مفاهیم Alignment و Padding آشنا شدیم و دیدیم که چطور می‌توانیم با تغییر ترتیب فیلدها در `struct` و ترکیب آن با `union` و `array`، فضای حافظه را بهینه‌تر کنیم. همچنین با استفاده از `#pragma pack` و `__attribute__ ((packed))` می‌توانیم کنترل بیشتری بر روی Padding داشته باشیم. استفاده از این تکنیک‌ها می‌تواند به بهینه‌تر شدن برنامه‌ها و کاهش مصرف حافظه منجر شود.

## تمرین‌ها


| شماره | سوال | بارم |
|-------|------|------|
| 1     | یک `struct` بساز که شامل یک `short`، یک `char` و یک `int` باشد. سایز این `struct` را محاسبه کن و دلیل وجود Padding را توضیح بده. | 3 |
| 2     | ساختار زیر را بهینه کن تا کمترین فضای حافظه را مصرف کند: `struct { char c; double d; int i; }`. | 4 |
| 3     | یک `union` بساز که شامل یک `int`، یک `float` و یک `char` باشد. این `union` را داخل یک `struct` قرار بده و بررسی کن که چطور Alignment و Padding روی اندازه کلی `struct` تأثیر می‌گذارند. | 5 |
| 4     | برنامه‌ای بنویس که سایز `struct` و `union` را در شرایط مختلف (با و بدون Padding) مقایسه کند. | 4 |
| 5     | از `#pragma pack` و `__attribute__ ((packed))` برای حذف Padding استفاده کن و بررسی کن که چطور این کار روی عملکرد برنامه تأثیر می‌گذارد. | 5 |

</div>