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

## درک مفهوم Callback: چیست و چرا؟

قبل از اینکه شروع به استفاده از Callback کنیم، بیا اول بفهمیم که دقیقاً چیه و چرا به دردمون می‌خوره.

### روش سنتی: چطور تا الان کار می‌کردیم؟

#### مثال متنی از زندگی روزمره

فرض کن یک آشپز داری که داره غذا می‌پزه. تو روش سنتی، باید مدام برو پیش آشپز و بپرسی: "غذا آماده شد؟" آشپز هم هر بار جواب می‌ده: "نه، هنوز نشده" یا "آره، آماده شد". این یعنی تو باید مدام کنترل کنی و منتظر بمونی.

#### مثال کدی: روش سنتی (Polling)

<div dir="ltr">

```c
// Traditional approach: Keep checking repeatedly
void traditional_cooking() {
    printf("Starting to cook...\n");
    
    // We have to keep checking if food is ready
    while (1) {
        if (is_food_ready()) {
            printf("Food is ready! Let's eat.\n");
            turn_off_stove();
            break;
        }
        printf("Still cooking... checking again in 1 second\n");
        delay(1000);  // Wait and check again
    }
}

int main() {
    traditional_cooking();  // We control everything
    return 0;
}
```

</div>

#### مشکلات روش سنتی

- **اتلاف وقت و انرژی:** مدام باید چک کنی که کار تموم شده یا نه
- **عدم کارایی:** حتی وقتی کار دیگه‌ای داری، باید برگردی و چک کنی
- **پیچیدگی:** اگه چند کار همزمان داشته باشی، مدیریت همه‌شون سخت می‌شه

### روش جدید: مفهوم callback چیست؟

#### مثال متنی با callback

حالا فرض کن به آشپز می‌گی: "من دارم می‌رم کار دیگه‌ای انجام بدم. وقتی غذا آماده شد، صدام بزن تا بیام". آشپز می‌گه: "باشه، وقتی آماده شد بهت خبر می‌دم". حالا تو می‌تونی برو کار دیگه‌ات رو انجام بدی و وقتی آشپز صدات زد، برگردی.

این همون مفهوم **callback** هست! به زبان ساده یعنی "بعداً صدام بزن".

#### مثال کدی: روش callback

<div dir="ltr">

```c
// Callback approach: Tell the system what to do when ready
void food_ready_notification() {
    printf("Food is ready! Let's eat.\n");
    turn_off_stove();
}

void cook_with_callback(void (*notify_when_ready)()) {
    printf("Starting to cook...\n");
    
    // Simulate cooking process
    delay(5000);  // Takes 5 seconds to cook
    
    // When food is ready, call the callback function
    if (notify_when_ready != NULL) {
        notify_when_ready();  // "Call me back" when ready!
    }
}

int main() {
    // Tell the cook: "When food is ready, call this function"
    cook_with_callback(food_ready_notification);
    
    // We can do other things here while food is cooking
    printf("I can do other tasks while food is cooking!\n");
    
    return 0;
}
```

</div>

### مقایسه: قبل و بعد از callback

#### روش سنتی (Polling):
- **تو کنترل می‌کنی:** مدام باید چک کنی
- **اتلاف منابع:** حتی وقتی کاری نیست، باید چک کنی  
- **محدودیت:** نمی‌تونی کار دیگه‌ای انجام بدی

#### روش callback:
- **سیستم بهت خبر می‌ده:** وقتی کار تموم شد، خودش صدات می‌زنه
- **بهینه‌تر:** فقط وقتی لازمه، کد اجرا می‌شه
- **انعطاف‌پذیر:** می‌تونی کارهای دیگه انجام بدی

### چطور callback منطق رو تغییر می‌ده؟

#### تغییر اساسی در نگرش:

**قبل:** "من باید مدام کنترل کنم"
```c
while (condition_not_met) {
    check_condition();
    wait();
}
do_action();
```

**بعد:** "وقتی شرایط آماده شد، این کار رو انجام بده"
```c
register_callback(do_action_when_ready);
// System will call do_action_when_ready() automatically
```

#### مثال عملی: سیستم هشدار

**روش سنتی:**
<div dir="ltr">

```c
// Traditional: We keep checking
void monitor_temperature_traditional() {
    while (1) {
        float temp = read_temperature();
        if (temp > 30.0) {
            printf("Temperature too high: %.1f\n", temp);
            turn_on_fan();
        }
        delay(1000);  // Check every second
    }
}
```

</div>

**روش callback:**
<div dir="ltr">

```c
// Callback: System notifies us when needed
void handle_high_temperature(float temp) {
    printf("Temperature too high: %.1f\n", temp);
    turn_on_fan();
}

void monitor_temperature_callback() {
    // Register what to do when temperature is high
    set_temperature_callback(handle_high_temperature, 30.0);
    
    // Now we can do other things!
    // System will automatically call handle_high_temperature() when needed
}
```

</div>

### چرا این تغییر مهمه؟

مکانیزم callback منطق برنامه‌نویسی رو از **"من کنترل می‌کنم"** به **"سیستم بهم خبر می‌ده"** تغییر می‌ده. این تغییر باعث می‌شه:

- **کد ساده‌تر بشه:** نیازی به حلقه‌های پیچیده نیست
- **کارایی بالاتر:** فقط وقتی لازمه، کد اجرا می‌شه  
- **انعطاف‌پذیری بیشتر:** می‌تونی چندین کار همزمان مدیریت کنی

<div dir="ltr">

```c
// Simple example: Cook and Callback
void food_ready_notification() {
    printf("Food is ready! Come and eat.\n");
}

void cook_food(void (*notify_when_ready)()) {
    printf("Cooking food...\n");
    // Assume it takes 5 seconds
    delay(5000);
    
    // Now that food is ready, call the callback
    if (notify_when_ready != NULL) {
        notify_when_ready();
    }
}

int main() {
    // Tell the cook to call this function when food is ready
    cook_food(food_ready_notification);
    return 0;
}
```

</div>

### فلسفه و منطق callback

#### 1. **جداسازی مسئولیت‌ها**

روش بدون callback:
- تابع `cook_food` باید بدونه غذا آماده شده چیکار کنه
- اگه بخوایم کار متفاوتی انجام بدیم، باید کل تابع رو تغییر بدیم

روش با callback:
- تابع `cook_food` فقط مسئول پختن غذاست
- چیکار کردن بعد از آماده شدن غذا، مسئولیت تابع دیگه‌ای هست

#### 2. **انعطاف‌پذیری**

<div dir="ltr">

```c
void send_sms_notification() {
    printf("SMS: Your food is ready!\n");
}

void play_alarm_sound() {
    printf("♪♪♪ Alarm: Food is ready! ♪♪♪\n");
}

void turn_off_stove() {
    printf("Stove turned off.\n");
}

int main() {
    // Today I want to receive SMS
    cook_food(send_sms_notification);
    
    // Tomorrow I want to hear alarm sound
    cook_food(play_alarm_sound);
    
    // Day after tomorrow I just want stove to turn off
    cook_food(turn_off_stove);
    
    return 0;
}
```

</div>

### چرا callback تو سیستم‌های نهفته مهمه؟

#### 1. **برنامه‌نویسی مبتنی بر رویداد**
سیستم‌های نهفته معمولاً بر اساس رویداد کار می‌کنن:
- سنسور دما بالا رفت → کولر روشن شه
- کلید فشرده شد → چراغ روشن شه  
- تایمر به پایان رسید → موتور خاموش شه

#### 2. **پاسخ بلادرنگ**
تو سیستم‌های نهفته، سرعت واکنش خیلی مهمه. مکانیزم callback باعث می‌شه بلافاصله بعد از یک رویداد، واکنش مناسب نشون بدیم.

#### 3. **بهینه‌سازی حافظه**
به جای اینکه کلی کد شرطی بنویسیم، فقط یک اشاره‌گر به تابع نگه می‌داریم که خیلی کم حافظه مصرف می‌کنه.

### مقایسه: قبل و بعد از callback

**بدون callback (سخت و غیرقابل انعطاف):**

<div dir="ltr">

```c
void handle_sensor_data(float temperature, int action_type) {
    if (action_type == 1) {
        printf("Fan turned on\n");
    } else if (action_type == 2) {
        printf("AC turned on\n");
    } else if (action_type == 3) {
        printf("Alert sent\n");
    }
    // If we want to add new action, we need to modify the whole function!
}
```

</div>

**با callback (انعطاف‌پذیر و قابل گسترش):**

<div dir="ltr">

```c
void handle_sensor_data(float temperature, void (*action_callback)(float)) {
    if (action_callback != NULL) {
        action_callback(temperature);
    }
}

// Each of these functions can be used as callback
void turn_on_fan(float temp) { printf("Fan turned on for temp %.1f\n", temp); }
void turn_on_ac(float temp) { printf("AC turned on for temp %.1f\n", temp); }
void send_alert(float temp) { printf("Alert: Temperature is %.1f!\n", temp); }
```

</div>

## استفاده از الگوی طراحی callback

حالا که فهمیدیم callback چیه و چرا مفیده، بیا ببینیم چطور می‌تونیم ازش تو سیستم تهویه مطبوع استفاده کنیم تا کدمون رو انعطاف‌پذیرتر و قابل گسترش‌تر کنیم.

### پیاده‌سازی ماژول مدیریت سنسور با پشتیبانی از callback

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

### پیاده‌سازی توابع callback تو برنامه اصلی

<div dir="ltr">

```c
// main.c
#include "sensor_manager.h"
#include <stdio.h>

void handle_high_temperature(float current_temperature) {
    printf("Warning: Temperature is %.2f! Taking action...\n", current_temperature);
    // Take necessary action, e.g., turn on fan
}

int main(void) {
    SensorManager temperature_manager;
    init_sensor_manager(&temperature_manager, 1, 30.0);

    set_temperature_callback(&temperature_manager, handle_high_temperature);

    while (1) {
        check_temperature(&temperature_manager);
        delay(1000); // Delay for loop speed control
    }

    return 0;
}
```

</div>

### مزایای استفاده از callback

- **انعطاف‌پذیری بالا:** می‌تونیم به راحتی رفتار سیستم رو تغییر بدیم، بدون اینکه نیاز باشه کد اصلی رو تغییر بدیم.
- **قابلیت گسترش آسان:** اضافه کردن سنسورهای جدید فقط با تعریف یک `SensorManager` جدید و تنظیم callback مربوطه انجام میشه.
- **کد مرتب‌تر و قابل نگهداری‌تر:** تصمیم‌گیری‌ها و واکنش‌ها تو توابع جداگانه مدیریت میشه و کد ساده‌تر میشه.

## گسترش سناریو: مدیریت چند سنسور

حالا که با مفهوم پایه callback آشنا شدیم، بیا سیستم تهویه مطبوع رو گسترش بدیم. فرض کن علاوه بر سنسور دما، یک سنسور رطوبت هم داریم و می‌خوایم هر دوتاشون رو با callback های جداگانه مدیریت کنیم.

### پیاده‌سازی سیستم دو سنسوره

<div dir="ltr">

```c
// sensor_manager.h
#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

typedef void (*TemperatureCallback)(float temperature);
typedef void (*HumidityCallback)(float humidity);

typedef struct {
    int sensor_id;
    float temperature_threshold;
    float humidity_threshold;
    TemperatureCallback on_high_temperature;
    HumidityCallback on_high_humidity;
} DualSensorManager;

void init_dual_sensor_manager(DualSensorManager* manager, int sensor_id, 
                             float temp_threshold, float humidity_threshold);
void set_temperature_callback(DualSensorManager* manager, TemperatureCallback callback);
void set_humidity_callback(DualSensorManager* manager, HumidityCallback callback);
void check_both_sensors(DualSensorManager* manager);

#endif
```

</div>

### پیاده‌سازی ماژول مدیریت دو سنسور

<div dir="ltr">

```c
// sensor_manager.c
#include "sensor_manager.h"
#include <stdio.h>

void init_dual_sensor_manager(DualSensorManager* manager, int sensor_id, 
                             float temp_threshold, float humidity_threshold) {
    manager->sensor_id = sensor_id;
    manager->temperature_threshold = temp_threshold;
    manager->humidity_threshold = humidity_threshold;
    manager->on_high_temperature = NULL;
    manager->on_high_humidity = NULL;
}

void set_temperature_callback(DualSensorManager* manager, TemperatureCallback callback) {
    manager->on_high_temperature = callback;
}

void set_humidity_callback(DualSensorManager* manager, HumidityCallback callback) {
    manager->on_high_humidity = callback;
}

void check_both_sensors(DualSensorManager* manager) {
    float temperature = read_temperature_sensor(manager->sensor_id);
    float humidity = read_humidity_sensor(manager->sensor_id);
    
    if (temperature > manager->temperature_threshold && manager->on_high_temperature != NULL) {
        manager->on_high_temperature(temperature);
    }
    
    if (humidity > manager->humidity_threshold && manager->on_high_humidity != NULL) {
        manager->on_high_humidity(humidity);
    }
}
```

</div>

### استفاده از سیستم دو سنسوره

<div dir="ltr">

```c
// main.c
#include "sensor_manager.h"
#include <stdio.h>

void handle_high_temperature(float temperature) {
    printf("Warning: Temperature is %.1f degrees! Fan will turn on.\n", temperature);
    // Turn on fan
}

void handle_high_humidity(float humidity) {
    printf("Warning: Humidity is %.1f percent! Dehumidifier will turn on.\n", humidity);
    // Turn on dehumidifier
}

int main(void) {
    DualSensorManager sensor_manager;
    init_dual_sensor_manager(&sensor_manager, 1, 30.0, 70.0);
    
    set_temperature_callback(&sensor_manager, handle_high_temperature);
    set_humidity_callback(&sensor_manager, handle_high_humidity);
    
    while (1) {
        check_both_sensors(&sensor_manager);
        delay(1000);
    }
    
    return 0;
}
```

</div>

### مزایای این روش

- **مدیریت مستقل:** هر سنسور callback مخصوص خودش رو داره
- **انعطاف‌پذیری:** می‌تونی فقط یکی از callback ها رو فعال کنی
- **گسترش آسان:** اضافه کردن سنسور جدید خیلی ساده‌ست

## مدیریت چند اتاق با callback

حالا فرض کن ساختمون ما چند اتاق داره و می‌خوایم هر اتاق رو جداگانه مدیریت کنیم. این یک سناریوی خیلی واقعی‌ست که تو ساختمون‌های هوشمند استفاده می‌شه.

### پیاده‌سازی سیستم چند اتاقه

<div dir="ltr">

```c
// room_manager.h
#ifndef ROOM_MANAGER_H
#define ROOM_MANAGER_H

typedef void (*RoomCallback)(const char* room_name, float temperature);

typedef struct {
    char room_name[32];
    int sensor_id;
    float temperature_threshold;
    RoomCallback room_callback;
} RoomSensor;

void init_room_sensor(RoomSensor* room, const char* name, int sensor_id, 
                     float threshold, RoomCallback callback);
void check_room_temperature(RoomSensor* room);
void check_all_rooms(RoomSensor* rooms, int room_count);

#endif
```

</div>

### پیاده‌سازی مدیریت اتاق‌ها

<div dir="ltr">

```c
// room_manager.c
#include "room_manager.h"
#include <stdio.h>
#include <string.h>

void init_room_sensor(RoomSensor* room, const char* name, int sensor_id, 
                     float threshold, RoomCallback callback) {
    strcpy(room->room_name, name);
    room->sensor_id = sensor_id;
    room->temperature_threshold = threshold;
    room->room_callback = callback;
}

void check_room_temperature(RoomSensor* room) {
    float temperature = read_temperature_sensor(room->sensor_id);
    
    if (temperature > room->temperature_threshold && room->room_callback != NULL) {
        room->room_callback(room->room_name, temperature);
    }
}

void check_all_rooms(RoomSensor* rooms, int room_count) {
    for (int i = 0; i < room_count; i++) {
        check_room_temperature(&rooms[i]);
    }
}
```

</div>

### استفاده از سیستم چند اتاقه

<div dir="ltr">

```c
// main.c
#include "room_manager.h"
#include <stdio.h>

void handle_living_room(const char* room_name, float temperature) {
    printf("🏠 %s: Temperature %.1f degrees! Main AC will turn on.\n", room_name, temperature);
}

void handle_bedroom(const char* room_name, float temperature) {
    printf("🛏️ %s: Temperature %.1f degrees! Ceiling fan will turn on.\n", room_name, temperature);
}

void handle_kitchen(const char* room_name, float temperature) {
    printf("🍳 %s: Temperature %.1f degrees! Kitchen exhaust will turn on.\n", room_name, temperature);
}

int main(void) {
    RoomSensor rooms[3];
    
    init_room_sensor(&rooms[0], "Living Room", 1, 26.0, handle_living_room);
    init_room_sensor(&rooms[1], "Bedroom", 2, 24.0, handle_bedroom);
    init_room_sensor(&rooms[2], "Kitchen", 3, 28.0, handle_kitchen);
    
    while (1) {
        check_all_rooms(rooms, 3);
        delay(2000);
    }
    
    return 0;
}
```

</div>

### چرا این روش عالیه؟

- ✅ **هر اتاق رفتار مخصوص خودش رو داره:** اتاق خواب فن سقفی، آشپزخانه هواکش
- ✅ **اضافه کردن اتاق جدید فقط 2 خط کد می‌خواد**
- ✅ **هر اتاق threshold مخصوص خودش رو داره:** اتاق خواب خنک‌تر، آشپزخانه گرم‌تر
- ✅ **کد تمیز و قابل فهم:** هر چیز تو جای مناسب خودش

## کنترل callback ها: خاموش و روشن کردن

گاهی وقت‌ها می‌خوایم بعضی از callback ها رو موقتاً غیرفعال کنیم. مثلاً شب‌ها نمی‌خوایم اعلان اتاق خواب بیاد و ما رو بیدار کنه!

### پیاده‌سازی سیستم کنترل‌پذیر

<div dir="ltr">

```c
// controllable_room.h
typedef struct {
    char room_name[32];
    int sensor_id;
    float temperature_threshold;
    RoomCallback room_callback;
    int callback_enabled;  // New: we can disable callback
} ControllableRoomSensor;

void toggle_room_callback(ControllableRoomSensor* room, int enable);
void check_controllable_room(ControllableRoomSensor* room);
```

</div>

### پیاده‌سازی کنترل callback

<div dir="ltr">

```c
void toggle_room_callback(ControllableRoomSensor* room, int enable) {
    room->callback_enabled = enable;
    printf("📱 Callback for %s is now %s\n", 
           room->room_name, 
           enable ? "ENABLED" : "DISABLED");
}

void check_controllable_room(ControllableRoomSensor* room) {
    float temperature = read_temperature_sensor(room->sensor_id);
    
    if (temperature > room->temperature_threshold && 
        room->callback_enabled && 
        room->room_callback != NULL) {
        room->room_callback(room->room_name, temperature);
    }
}
```

</div>

### مثال کاربردی: حالت شب

<div dir="ltr">

```c
void enable_night_mode(ControllableRoomSensor* rooms, int room_count) {
    for (int i = 0; i < room_count; i++) {
        if (strcmp(rooms[i].room_name, "Bedroom") == 0) {
            toggle_room_callback(&rooms[i], 0);  // Turn off bedroom
        } else {
            toggle_room_callback(&rooms[i], 1);  // Keep others on
        }
    }
    printf("🌙 Night mode activated!\n");
}

void disable_night_mode(ControllableRoomSensor* rooms, int room_count) {
    for (int i = 0; i < room_count; i++) {
        toggle_room_callback(&rooms[i], 1);  // Turn all on
    }
    printf("☀️ Day mode activated!\n");
}
```

</div>

## تمرین‌ها

### تمرین‌های پایه

| شماره | سوال                                                                                                                                                                                                                      | بارم |
| ----- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---- |
| 1     | یک تابع Callback تعریف کن که یک پیام متنی رو دریافت کنه و چاپش کنه. بعد یک تابع دیگه بنویس که این Callback رو به عنوان آرگومان دریافت کنه و اون رو فراخوانی کنه.                                                          | 1    |
| 2     | یک آرایه از اعداد صحیح تعریف کن و یک تابع Callback بنویس که اعداد رو پردازش کنه (مثلاً هر عدد رو یک واحد اضافه کنه). بعد این Callback رو به هر عنصر آرایه اعمال کن.                                                        | 2    |
| 3     | یک برنامه ساده بنویس که دو رویداد "کلید A فشرده شده" و "کلید B فشرده شده" رو مدیریت کنه. برای هر رویداد یک Callback تعریف کن و اون‌ها رو به توابع مربوطه متصل کن.                                                          | 3    |
| 4     | یک سناریوی ساده با سه نوع سنسور (دما، رطوبت و نور) تعریف کن. برای هر سنسور یک Callback بنویس که وقتی به یک حد مشخص رسید، اقدامات لازم رو انجام بده. سیستم باید بتونه هر کدوم از این توابع Callback رو جداگانه مدیریت کنه. | 5    |

### تمرین‌های پیشرفته

| شماره | سوال                                                                                                                                                                                                                                                                                                                                                                       | بارم |
| ----- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---- |
| 5     | سیستم تهویه مطبوع رو برای 2 اتاق (اتاق نشیمن و اتاق خواب) گسترش بده. هر اتاق باید Callback مخصوص خودش رو داشته باشه و threshold متفاوت (اتاق نشیمن: 26 درجه، اتاق خواب: 24 درجه).                                                                                                                                                                                      | 3    |
| 6     | به سیستم قبلی قابلیت خاموش/روشن کردن Callback ها رو اضافه کن. یک تابع "حالت شب" بنویس که Callback اتاق خواب رو غیرفعال کنه تا شب‌ها مزاحم نباشه.                                                                                                                                                                                                                           | 4    |
| 7     | یک سیستم هشدار آتش‌سوزی بساز که سنسورهای دود و حرارت داشته باشه. هر سنسور Callback مخصوص خودش رو داشته باشه (سنسور دود: روشن کردن آژیر، سنسور حرارت: فعال کردن سیستم اطفاء حریق). سیستم باید بتونه هر سنسور رو جداگانه کنترل کنه.                                                                                                                                      | 5    |
| 8     | یک سیستم مانیتورینگ گلخانه بساز که 3 نوع سنسور داشته باشه: دما، رطوبت و نور. هر سنسور باید دو حد داشته باشه (کمینه و بیشینه) و برای هر حد یک Callback جداگانه. مثلاً اگه دما خیلی پایین بود، بخاری روشن بشه و اگه خیلی بالا بود، فن روشن بشه.                                                                                                                   | 6    |
| 9     | یک سیستم خانه هوشمند کامل بساز که شامل: سنسور حرکت (روشن کردن چراغ)، سنسور درب و پنجره (هشدار امنیتی)، سنسور دما (کنترل تهویه) باشه. سیستم باید یک "حالت مهمان" داشته باشه که فقط چراغ‌ها کار کنن و سنسورهای امنیتی غیرفعال بشن.                                                                                                                                        | 7    |
| 10    | یک سیستم مانیتورینگ ماشین صنعتی بساز که سنسورهای مختلفی مثل دما، ارتعاش، فشار و سرعت داشته باشه. هر سنسور باید سه سطح هشدار داشته باشه: عادی، هشدار و خطرناک. برای هر سطح یک Callback متفاوت تعریف کن و سیستم باید بتونه در صورت بروز مشکل، ماشین رو خاموش کنه.                                                                                                | 8    |

</div>
