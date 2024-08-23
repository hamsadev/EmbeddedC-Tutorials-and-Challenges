<div dir="rtl">

# استفاده از Factory Method برای مدیریت سنسورها تو سیستم‌های نهفته

این راهنما قراره بهت نشون بده که چطور با استفاده از الگوی طراحی Factory Method می‌تونی یه سیستم کنترل سنسور خیلی مرتب و انعطاف‌پذیر بسازی. علاوه بر این، آخرش یه سری تمرین هم برات گذاشتم که بتونی مهارت‌هات رو تو این زمینه تقویت کنی.

## سناریو: کنترل چندین سنسور مختلف

فرض کن داری یه سیستم نهفته طراحی می‌کنی که باید چندین نوع سنسور مختلف رو کنترل کنه. مثلاً سنسورهای دما، فشار و رطوبت که هر کدومشون از یه پروتکل ارتباطی مختلف (مثل I2C، SPI و UART) استفاده می‌کنن. هدفت اینه که این سنسورها رو مدیریت کنی و در عین حال بتونی سنسورهای جدید رو به راحتی اضافه کنی.

### مرحله 1: پیاده‌سازی ساده (بدون Factory Method)

اول بیا ببینیم اگه بخوای این مسئله رو بدون Factory Method حل کنی، چه مشکلاتی پیش میاد.

#### 1.1 تعریف توابع برای هر سنسور

فرض کن باید برای هر نوع سنسور، یه سری توابع جداگانه برای مقداردهی اولیه (init) و خواندن داده (read_data) بنویسی. کدش ممکنه این شکلی باشه:

<div dir="ltr">

```c
// sensor_manager.c
#include <stdio.h>

// توابع برای سنسور I2C
void init_I2CSensor(void) {
    // کدهای مقداردهی اولیه برای I2C
}

int read_I2CSensor(void) {
    // کد برای خواندن داده از سنسور I2C
    return 0;
}

// توابع برای سنسور SPI
void init_SPISensor(void) {
    // کدهای مقداردهی اولیه برای SPI
}

int read_SPISensor(void) {
    // کد برای خواندن داده از سنسور SPI
    return 0;
}

// توابع برای سنسور UART
void init_UARTSensor(void) {
    // کدهای مقداردهی اولیه برای UART
}

int read_UARTSensor(void) {
    // کد برای خواندن داده از سنسور UART
    return 0;
}

```

</div>

#### 1.2 استفاده از این توابع تو برنامه اصلی

برای استفاده از این سنسورها، باید نوع سنسور و توابع مربوط بهش رو تو کد اصلی مشخص کنی.

<div dir="ltr">

```c
// main.c
#include "sensor_manager.h"

int main(void) {
    // مقداردهی اولیه سنسور I2C
    init_I2CSensor();

    // خواندن داده از سنسور I2C
    int data = read_I2CSensor();

    while (1) {
        // حلقه اصلی برنامه
    }

    return 0;
}

```

</div>

### مشکلات روش بدون Factory Method

1. **کد تکراری:** برای هر نوع سنسور، باید توابع جداگانه‌ای برای مقداردهی اولیه و خواندن داده تعریف کنی. این باعث میشه که کد تکراری زیادی داشته باشی.

2. **عدم انعطاف‌پذیری:** اگه بخوای نوع سنسور یا پروتکل ارتباطی رو تغییر بدی، باید کدهای زیادی رو تو برنامه اصلی تغییر بدی.

3. **افزودن سنسور جدید:** اگه بخوای یه سنسور جدید اضافه کنی، باید توابع جدیدی براش تعریف کنی و برنامه اصلی رو هم تغییر بدی. این کار باعث میشه کدت پیچیده و نگهداریش سخت بشه.

### مرحله 2: استفاده از Factory Method

حالا بیا ببینیم چطور می‌تونیم با استفاده از Factory Method این مشکلات رو حل کنیم.

#### 2.1 تعریف یه ساختار پایه برای سنسورها

اول یه ساختار پایه برای سنسورها تعریف می‌کنیم که شامل توابعی مثل `init()` و `read_data()` باشه. اینجوری می‌تونیم با هر نوع سنسوری به یه شکل برخورد کنیم.

<div dir="ltr">

```c
// sensor.h
#ifndef SENSOR_H
#define SENSOR_H

typedef struct {
    void (*init)(void);
    int (*read_data)(void);
} Sensor;

Sensor* createI2CSensor(void);
Sensor* createSPISensor(void);
Sensor* createUARTSensor(void);

#endif
```

</div>

#### 2.2 تعریف توابع Factory برای هر سنسور

حالا برای هر نوع سنسور، یه تابع Factory تعریف می‌کنیم که یه شیء `Sensor` بسازه و توابع مربوط به اون سنسور رو بهش اختصاص بده.

<div dir="ltr">

```c
// sensor.c
#include "sensor.h"
#include <stdlib.h>

void I2C_init(void) {
    // کدهای مقداردهی اولیه برای I2C
}

int I2C_read_data(void) {
    // کد برای خواندن داده از سنسور I2C
    return 0;
}

Sensor* createI2CSensor(void) {
    Sensor* sensor = (Sensor*)malloc(sizeof(Sensor));
    sensor->init = I2C_init;
    sensor->read_data = I2C_read_data;
    return sensor;
}

void SPI_init(void) {
    // کدهای مقداردهی اولیه برای SPI
}

int SPI_read_data(void) {
    // کد برای خواندن داده از سنسور SPI
    return 0;
}

Sensor* createSPISensor(void) {
    Sensor* sensor = (Sensor*)malloc(sizeof(Sensor));
    sensor->init = SPI_init;
    sensor->read_data = SPI_read_data;
    return sensor;
}

void UART_init(void) {
    // کدهای مقداردهی اولیه برای UART
}

int UART_read_data(void) {
    // کد برای خواندن داده از سنسور UART
    return 0;
}

Sensor* createUARTSensor(void) {
    Sensor* sensor = (Sensor*)malloc(sizeof(Sensor));
    sensor->init = UART_init;
    sensor->read_data = UART_read_data;
    return sensor;
}

```

</div>

#### 2.3 استفاده از Factory Method تو برنامه اصلی

حالا می‌تونیم بدون تغییر تو کد اصلی، سنسورهای مختلف رو به سادگی مدیریت کنیم.

<div dir="ltr">

```c
// main.c
#include "sensor.h"

int main(void) {
    Sensor* mySensor;

    // انتخاب سنسور بر اساس نیاز
    mySensor = createI2CSensor();  // یا createSPISensor() یا createUARTSensor()

    // مقداردهی اولیه سنسور
    mySensor->init();

    // خواندن داده از سنسور
    int data = mySensor->read_data();

    // پاکسازی حافظه
    free(mySensor);

    while (1) {
        // حلقه اصلی برنامه
    }

    return 0;
}
```

</div>

### مزایای استفاده از Factory Method

1. **انعطاف‌پذیری بالا:** اضافه کردن سنسورهای جدید یا تغییر پروتکل ارتباطی بدون نیاز به تغییر تو کد اصلی انجام میشه.

2. **کد مرتب‌تر و نگهداری راحت‌تر:** همه سنسورها از یه ساختار پایه استفاده می‌کنن که باعث میشه کد تمیزتر و نگهداریش راحت‌تر باشه.

3. **قابلیت گسترش:** به راحتی می‌تونی سنسورهای جدیدی اضافه کنی و فقط باید یه تابع Factory جدید براش تعریف کنی.

## تمرین‌ها

| شماره | سوال | بارم |
|-------|------|------|
| 1     | یه سنسور جدید به اسم `createUARTSensor` اضافه کن که از پروتکل UART استفاده می‌کنه. کدهای مربوط به مقداردهی اولیه و خواندن داده از سنسور رو بنویس. | 2 |
| 2     | برنامه اصلی رو طوری تغییر بده که بتونه به صورت پویا سنسور مورد نیاز رو بر اساس ورودی کاربر (مثلاً I2C یا SPI) انتخاب کنه. | 3 |
| 3     | یه سنسور جدید به اسم `createAnalogSensor` درست کن که از ورودی آنالوگ استفاده می‌کنه. | 4 |
| 4     | کد رو طوری بهینه کن که بتونه به راحتی تعداد زیادی سنسور از انواع مختلف رو مدیریت کنه. برای هر سنسور یه نمونه از کلاس پایه ایجاد کن و بهش اضافه کن. | 5 |


</div>