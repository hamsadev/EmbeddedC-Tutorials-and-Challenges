<div dir="rtl">

# الگوی طراحی Callback در سیستم‌های نهفته

این README قراره بهت کمک کنه تا با الگوی طراحی Callback تو سیستم‌های نهفته آشنا بشی. همچنین شامل یک سری تمرین از ساده تا پیچیده‌ است که می‌تونی با انجامشون مهارت‌هات رو تو این زمینه تقویت کنی.

## سناریو: مدیریت داده‌های سنسور در سیستم تهویه مطبوع هوشمند

فرض کن داری یک سیستم تهویه مطبوع هوشمند برای یک ساختمون طراحی می‌کنی. این سیستم باید بتونه داده‌های مختلفی رو از سنسورها (مثل دما، رطوبت و کیفیت هوا) بگیره و بر اساس اون‌ها تهویه رو کنترل کنه. مثلا وقتی دما میره بالا، تهویه باید سریع‌تر کار کنه تا محیط خنک بشه.

### پیاده‌سازی بدون استفاده از Callback

اول بیا مسئله رو بدون استفاده از الگوی طراحی Callback پیاده‌سازی کنیم. تو این روش، همه چیز تو یک ماژول متمرکز شده و تصمیم‌گیری‌ها مستقیماً تو کد نوشته می‌شن.

#### 1. تعریف ساختار مدیریت سنسور

<div dir="ltr">

```c
// sensor_manager.h
#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

typedef struct {
    int sensor_id;
    float current_temperature;
    float threshold;
} SensorManager;

void init_sensor_manager(SensorManager* manager, int sensor_id, float threshold);
void check_temperature(SensorManager* manager);

#endif
```
</div>

#### 2. پیاده‌سازی ماژول مدیریت سنسور

<div dir="ltr">

```c
// sensor_manager.c
#include "sensor_manager.h"
#include <stdio.h>

void init_sensor_manager(SensorManager* manager, int sensor_id, float threshold) {
    manager->sensor_id = sensor_id;
    manager->threshold = threshold;
}

void check_temperature(SensorManager* manager) {
    // فرض کن این تابع دما رو از سنسور می‌خونه
    manager->current_temperature = read_sensor(manager->sensor_id);

    if (manager->current_temperature > manager->threshold) {
        printf("Warning: Temperature is too high! Taking action...
");
        // اینجا مثلاً فن رو روشن می‌کنیم یا هر اقدام دیگه‌ای که لازمه
    }
}
```
</div>

#### 3. استفاده از ماژول تو برنامه اصلی

<div dir="ltr">

```c
// main.c
#include "sensor_manager.h"

int main(void) {
    SensorManager temperature_manager;
    init_sensor_manager(&temperature_manager, 1, 30.0);

    while (1) {
        check_temperature(&temperature_manager);
        delay(1000); // تأخیر برای کنترل سرعت حلقه
    }

    return 0;
}
```

</div>

### مشکلات روش بدون Callback

- **وابستگی مستقیم:** اگه بخوایم رفتار سیستم رو تغییر بدیم، باید مستقیماً تو کد اصلی تغییرات ایجاد کنیم.
- **قابلیت گسترش محدود:** اگه بخوایم سنسورهای جدید اضافه کنیم یا رفتار سنسورهای فعلی رو تغییر بدیم، نیاز به تغییرات زیادی تو کد داریم.
- **نگهداری سخت:** تغییرات مکرر باعث میشه کد پیچیده بشه و نگهداریش سخت بشه.

## استفاده از الگوی طراحی Callback

حالا بیا با استفاده از الگوی طراحی Callback کد رو انعطاف‌پذیرتر و قابل گسترش‌تر کنیم.

### پیاده‌سازی ماژول مدیریت سنسور با پشتیبانی از Callback

<div dir="ltr">

```c
// sensor_manager.h
#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

typedef void (*TemperatureCallback)(float current_temperature);

typedef struct {
    int sensor_id;
    float threshold;
    TemperatureCallback on_threshold_exceed;
} SensorManager;

void init_sensor_manager(SensorManager* manager, int sensor_id, float threshold);
void set_temperature_callback(SensorManager* manager, TemperatureCallback callback);
void check_temperature(SensorManager* manager);

#endif
```

</div>

### پیاده‌سازی ماژول مدیریت سنسور

<div dir="ltr">

```c
// sensor_manager.c
#include "sensor_manager.h"
#include <stdio.h>

void init_sensor_manager(SensorManager* manager, int sensor_id, float threshold) {
    manager->sensor_id = sensor_id;
    manager->threshold = threshold;
    manager->on_threshold_exceed = NULL;
}

void set_temperature_callback(SensorManager* manager, TemperatureCallback callback) {
    manager->on_threshold_exceed = callback;
}

void check_temperature(SensorManager* manager) {
    float current_temperature = read_sensor(manager->sensor_id);

    if (current_temperature > manager->threshold && manager->on_threshold_exceed != NULL) {
        manager->on_threshold_exceed(current_temperature);
    }
}
```

</div>

### پیاده‌سازی توابع Callback تو برنامه اصلی

<div dir="ltr">

```c
// main.c
#include "sensor_manager.h"
#include <stdio.h>

void handle_high_temperature(float current_temperature) {
    printf("Warning: Temperature is %.2f! Taking action...
", current_temperature);
    // اقدام لازم، مثلاً روشن کردن فن
}

int main(void) {
    SensorManager temperature_manager;
    init_sensor_manager(&temperature_manager, 1, 30.0);

    set_temperature_callback(&temperature_manager, handle_high_temperature);

    while (1) {
        check_temperature(&temperature_manager);
        delay(1000); // تأخیر برای کنترل سرعت حلقه
    }

    return 0;
}
```

</div>

### مزایای استفاده از Callback

- **انعطاف‌پذیری بالا:** می‌تونیم به راحتی رفتار سیستم رو تغییر بدیم، بدون اینکه نیاز باشه کد اصلی رو تغییر بدیم.
- **قابلیت گسترش آسان:** اضافه کردن سنسورهای جدید فقط با تعریف یک `SensorManager` جدید و تنظیم Callback مربوطه انجام میشه.
- **کد مرتب‌تر و قابل نگهداری‌تر:** تصمیم‌گیری‌ها و واکنش‌ها تو توابع جداگانه مدیریت میشه و کد ساده‌تر میشه.

## تمرین‌ها

| شماره | سوال                                                                                                                                                                                                                      | بارم |
| ----- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---- |
| 1     | یک تابع Callback تعریف کن که یک پیام متنی رو دریافت کنه و چاپش کنه. بعد یک تابع دیگه بنویس که این Callback رو به عنوان آرگومان دریافت کنه و اون رو فراخوانی کنه.                                                          | 1    |
| 2     | یک آرایه از اعداد صحیح تعریف کن و یک تابع Callback بنویس که اعداد رو پردازش کنه (مثلاً هر عدد رو یک واحد اضافه کنه). بعد این Callback رو به هر عنصر آرایه اعمال کن.                                                        | 2    |
| 3     | یک برنامه ساده بنویس که دو رویداد "کلید A فشرده شده" و "کلید B فشرده شده" رو مدیریت کنه. برای هر رویداد یک Callback تعریف کن و اون‌ها رو به توابع مربوطه متصل کن.                                                          | 3    |
| 4     | یک سناریوی ساده با سه نوع سنسور (دما، رطوبت و نور) تعریف کن. برای هر سنسور یک Callback بنویس که وقتی به یک حد مشخص رسید، اقدامات لازم رو انجام بده. سیستم باید بتونه هر کدوم از این توابع Callback رو جداگانه مدیریت کنه. | 5    |

</div>
