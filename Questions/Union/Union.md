<div dir="rtl">

`Union` یکی از ویژگی‌های مهم و کاربردی زبان C هست که به شما این امکان رو می‌ده چندین نوع داده رو در یک مکان حافظه مشترک ذخیره کنید. این قابلیت توی سیستم‌های نهفته (Embedded Systems) که حافظه محدوده، اهمیت بیشتری پیدا می‌کنه. بیایید با استفاده از چند سناریو عملی، بفهمیم چطور `union` می‌تونه کدمون رو بهینه‌تر و مدیریت حافظه رو کارآمدتر کنه.

---

## تعریف Union با `typedef`

با `typedef` می‌تونیم یک `union` تعریف کنیم که از چندین نوع داده تشکیل شده. نکته اینجاست که تو هر لحظه فقط یکی از این داده‌ها می‌تونه فعال باشه و استفاده بشه.

**مثال تعریف union:**

<div dir="ltr">

```c
typedef union {
    int i;
    float f;
    char str[20];
} Data;
```

</div>

این `union` شامل سه نوع داده مختلفه: `int`، `float` و `char[]`. اما فقط یکی از این‌ها در هر لحظه می‌تونه مقداری داشته باشه. 

---

## محاسبه سایز `union`

سایز `union` بر اساس بزرگ‌ترین عضو اون تعیین می‌شه. یعنی هر عضو از `union` توی همون فضای حافظه قرار می‌گیره، ولی سایز کل `union` برابر با بزرگ‌ترین عضوش هست.

### مثال:

<div dir="ltr">

```c
typedef union {
    int i;          // 4 bytes
    float f;        // 4 bytes
    char str[20];   // 20 bytes
} Data;
```

</div>

در اینجا، `int` و `float` هر کدوم 4 بایت فضا می‌گیرن، ولی `str` که 20 بایته بزرگ‌ترین عضو این `union` هست. بنابراین سایز کل `union` برابر با 20 بایت خواهد بود.

---

## سناریو 1: مدیریت موجودی در یک انبار هوشمند

فرض کنید در یک انبار هوشمند باید کالاها رو بر اساس وزن، تعداد یا حجم مدیریت کنید. حالا بیایید این کار رو بدون استفاده از `union` انجام بدیم و مشکلاتش رو ببینیم.

### پیاده‌سازی بدون استفاده از Union

ابتدا سه تا `struct` برای هر نوع کالا جداگانه تعریف می‌کنیم:

<div dir="ltr">

```c
typedef struct {  
    char name[30];  
    float weight;
} WeightItem;

typedef struct {  
    char name[30];  
    int quantity;
} QuantityItem;

typedef struct {  
    char name[30];  
    float volume;
} VolumeItem;
```

</div>

حالا برای هر نوع کالا، یک ساختار و تابع جداگانه برای چاپ اطلاعات کالا داریم:

<div dir="ltr">

```c
void printWeightItem(WeightItem item) {
    printf("Item: %s, Weight: %.2f kg\n", item.name, item.weight);
}

void printQuantityItem(QuantityItem item) {
    printf("Item: %s, Quantity: %d units\n", item.name, item.quantity);
}

void printVolumeItem(VolumeItem item) {
    printf("Item: %s, Volume: %.2f cubic meters\n", item.name, item.volume);
}
```

</div>

حالا فرض کنید کالاهایی مثل سیب، صندلی و گاز طبیعی داریم و می‌خوایم اطلاعاتشون رو چاپ کنیم:

<div dir="ltr">

```c
int main() {
    WeightItem apple = {"Apple", 1.5};
    QuantityItem chair = {"Chair", 10};
    VolumeItem gas = {"Natural Gas", 5.3};

    printWeightItem(apple);
    printQuantityItem(chair);
    printVolumeItem(gas);

    return 0;
}
```

</div>

### مشکلات روش بدون Union

1. **تکرار کد:** برای هر نوع کالا باید `struct` و تابع‌های جداگانه تعریف کنیم.
2. **مصرف بیشتر حافظه:** هر `struct` فضای جداگانه‌ای توی حافظه داره، حتی اگه فقط یکی از اعضا استفاده بشه.

### مشکل واقعی: پیچیدگی مدیریت کالاهای جدید

تصور کنید بخوایم کالاهای جدیدی مثل کالاهای **قیمت‌دار** اضافه کنیم. در این صورت، باید یک ساختار جدید بنویسیم و تابع جداگانه‌ای برای چاپ یا مدیریت قیمت کالاها هم اضافه کنیم:

<div dir="ltr">

```c
typedef struct {
    char name[30];
    float price;
} PriceItem;

void printPriceItem(PriceItem item) {
    printf("Item: %s, Price: %.2f\n", item.name, item.price);
}
```

</div>

همین‌طور که تعداد کالاهای جدید بیشتر بشه، ساختارها و تابع‌های بیشتری لازم داریم، و این باعث می‌شه کد پیچیده و نگهداریش سخت بشه.

---

### بهینه‌سازی با استفاده از Union

حالا بیایم همین سناریو رو با استفاده از `union` و `enum` بهینه‌تر کنیم. هدف ما اینه که برای هر نوع کالا از یک ساختار و تابع عمومی استفاده کنیم و حافظه رو بهتر مدیریت کنیم.

#### تعریف ساختار با `union`

<div dir="ltr">

```c
typedef enum {  
    WEIGHT_BASED,   
    QUANTITY_BASED,  
    VOLUME_BASED,  
    PRICE_BASED    
} ItemType;

typedef union {  
    float weight;  
    int quantity;  
    float volume;  
    float price;  
} InventoryItem;

typedef struct {  
    char name[30];  
    ItemType type;  
    InventoryItem data;  
} Item;
```

</div>

حالا می‌تونیم برای تمام کالاها از یک ساختار واحد (`Item`) استفاده کنیم که هم وزن، هم تعداد، هم حجم و هم قیمت رو توی یک `union` نگه‌داری می‌کنه.

#### تابع عمومی برای چاپ اطلاعات

حالا یک تابع عمومی برای چاپ اطلاعات کالاها تعریف می‌کنیم که بر اساس نوع کالا (وزنی، تعدادی، حجمی یا قیمت‌دار) اطلاعات رو چاپ می‌کنه:

<div dir="ltr">

```c
void printItem(Item item) {
    printf("Item: %s\n", item.name);
    switch (item.type) {
        case WEIGHT_BASED:
            printf("Weight: %.2f kg\n", item.data.weight);
            break;
        case QUANTITY_BASED:
            printf("Quantity: %d units\n", item.data.quantity);
            break;
        case VOLUME_BASED:
            printf("Volume: %.2f cubic meters\n", item.data.volume);
            break;
        case PRICE_BASED:
            printf("Price: %.2f\n", item.data.price);
            break;
        default:
            printf("Unknown item type\n");
    }
}
```

</div>

#### استفاده از ساختار `union` در کد اصلی

حالا با استفاده از این ساختار و تابع عمومی، می‌تونیم کالاهای مختلف رو بدون نوشتن کدهای تکراری مدیریت کنیم:

<div dir="ltr">

```c
int main() {
    Item apple = {"Apple", WEIGHT_BASED, .data.weight = 1.5};
    Item chair = {"Chair", QUANTITY_BASED, .data.quantity = 10};
    Item gas = {"Natural Gas", VOLUME_BASED, .data.volume = 5.3};
    Item book = {"Book", PRICE_BASED, .data.price = 15.99};

    printItem(apple);
    printItem(chair);
    printItem(gas);
    printItem(book);

    return 0;
}
```

</div>

### مزایای استفاده از Union:

1. **کاهش مصرف حافظه:** همه داده‌ها در یک فضای مشترک قرار می‌گیرن، یعنی از حافظه بهینه استفاده می‌شه.
2. **کد مرتب‌تر و مدیریت ساده‌تر:** فقط یک ساختار و یک تابع عمومی داریم که همه کالاها رو مدیریت می‌کنه. نیاز نیست برای هر کالای جدید، کد جدید بنویسیم.

---

## سناریو 2: دسترسی به بایت‌های کم‌ارزش و پر‌ارزش در `uint16_t`

خیلی وقت‌ها توی برنامه‌نویسی Embedded نیاز داریم که به بایت‌های کم‌ارزش (Low Byte) و پر‌ارزش (High Byte) یک متغیر 16 بیتی دسترسی داشته باشیم. این کار مخصوصاً وقتی نیاز به انتقال داده‌ها داریم، مثل ارتباط سریال، خیلی مفید هست.

**مثال:**

<div dir="ltr">

```c
typedef union {  
    uint16_t fullValue;  
    struct {  
        uint8_t lowByte;  
        uint8_t highByte;  
    };  
} Value16;
```

</div>

اینجا می‌تونیم هم به کل مقدار 16 بیتی دسترسی داشته باشیم و هم به بایت‌های کم‌ارزش و پر‌ارزش اون. حالا با استفاده از این ساختار، می‌تونیم بایت‌ها رو جداگانه تغییر بدیم.

---

## سناریو 3: مدیریت آرایه‌ای از Callback‌ها

فرض کنید توی سیستم نهفته‌تون نیاز دارید چندین
تابع مختلف رو با `callback` مدیریت کنید. می‌خواید به این تابع‌ها هم به صورت جداگانه و هم به عنوان یک آرایه دسترسی داشته باشید. اینجاست که `union` کمک می‌کنه.

### مثال:

<div dir="ltr">

```c
typedef void (*Callback)(void);

typedef union {
    struct {
        Callback callback1;
        Callback callback2;
        Callback callback3;
    };
    Callback callbacks[3];
} CallbackUnion;
```

</div>

با این `union` هم می‌تونیم به تابع‌های `callback` به صورت جداگانه دسترسی داشته باشیم و هم اون‌ها رو به شکل آرایه‌ای پردازش کنیم.

---

## مدیریت دستگاه‌های ورودی/خروجی (I/O) در یک سیستم نهفته

در این سناریو فرض کنید در یک سیستم نهفته (Embedded) کار می‌کنید که شامل چندین دستگاه ورودی و خروجی مختلف هست، مثل:
- **LED**: که روشن و خاموش می‌شه.
- **سنسور دما**: که دما رو اندازه‌گیری می‌کنه.
- **کلید (Button)**: که می‌تونه فشرده یا آزاد بشه.

هدف اینه که وضعیت این دستگاه‌ها رو مدیریت کنیم و با استفاده از `union` این کار رو بهینه‌تر و منعطف‌تر انجام بدیم.

---

## مرحله 1: پیاده‌سازی بدون استفاده از `union`

فرض کنید برای هر دستگاه ورودی/خروجی، یک `struct` تعریف کردیم که وضعیت اون رو نگه‌داری می‌کنه.

<div dir="ltr">

```c
typedef struct {
    int status;  // 0: خاموش، 1: روشن
} LED;

typedef struct {
    float temperature;  // دمای اندازه‌گیری شده
} TemperatureSensor;

typedef struct {
    int pressed;  // 0: آزاد، 1: فشرده
} Button;
```

</div>

برای مدیریت و نمایش وضعیت این دستگاه‌ها هم به چندین تابع جداگانه نیاز داریم.

<div dir="ltr">

```c
void printLEDStatus(LED led) {
    printf("LED is %s\n", led.status ? "ON" : "OFF");
}

void printTemperature(TemperatureSensor sensor) {
    printf("Temperature: %.2f°C\n", sensor.temperature);
}

void printButtonStatus(Button button) {
    printf("Button is %s\n", button.pressed ? "Pressed" : "Released");
}
```

</div>

حالا فرض کنید که باید وضعیت این سه دستگاه رو چاپ کنیم:

<div dir="ltr">

```c
int main() {
    LED led = {1};  // LED روشن
    TemperatureSensor sensor = {23.5};  // دما: 23.5°C
    Button button = {0};  // دکمه آزاد

    printLEDStatus(led);
    printTemperature(sensor);
    printButtonStatus(button);

    return 0;
}
```

</div>

### مشکلات این روش:
1. **تکرار کد:** برای هر دستگاه نیاز به `struct` و تابع جداگانه داریم که باعث تکرار کد می‌شه.
2. **مدیریت پیچیده:** اضافه کردن دستگاه‌های جدید، نیازمند اضافه کردن ساختار و تابع‌های بیشتره.

---

## مرحله 2: بهینه‌سازی با استفاده از Union

حالا می‌خوایم این سناریو رو با استفاده از `union` و `enum` بهینه‌تر کنیم.

### تعریف یک ساختار با Union

<div dir="ltr">

```c
typedef enum {
    DEVICE_LED,
    DEVICE_TEMP_SENSOR,
    DEVICE_BUTTON
} DeviceType;

typedef union {
    int status;          // برای LED و Button
    float temperature;   // برای سنسور دما
} DeviceData;

typedef struct {
    char name[20];       // نام دستگاه
    DeviceType type;     // نوع دستگاه
    DeviceData data;     // داده‌های دستگاه
} Device;
```

</div>

### تابع عمومی برای نمایش وضعیت دستگاه‌ها

<div dir="ltr">

```c
void printDeviceStatus(Device device) {
    printf("Device: %s\n", device.name);
    switch (device.type) {
        case DEVICE_LED:
            printf("LED is %s\n", device.data.status ? "ON" : "OFF");
            break;
        case DEVICE_TEMP_SENSOR:
            printf("Temperature: %.2f°C\n", device.data.temperature);
            break;
        case DEVICE_BUTTON:
            printf("Button is %s\n", device.data.status ? "Pressed" : "Released");
            break;
        default:
            printf("Unknown device type\n");
    }
}
```

</div>

### استفاده از Union در برنامه اصلی

حالا می‌تونیم وضعیت تمام دستگاه‌ها رو با استفاده از یک ساختار و یک تابع عمومی مدیریت کنیم:

<div dir="ltr">

```c
int main() {
    Device led = {"LED", DEVICE_LED, .data.status = 1};
    Device sensor = {"Temperature Sensor", DEVICE_TEMP_SENSOR, .data.temperature = 23.5};
    Device button = {"Button", DEVICE_BUTTON, .data.status = 0};

    printDeviceStatus(led);
    printDeviceStatus(sensor);
    printDeviceStatus(button);

    return 0;
}
```

</div>

### مزایای استفاده از Union در این سناریو:
1. **کاهش تکرار کد:** نیازی به ساختار و تابع جداگانه برای هر دستگاه نیست.
2. **کاهش مصرف حافظه:** دستگاه‌های مختلف در یک فضای حافظه مشترک نگه‌داری می‌شن.
3. **مدیریت ساده‌تر:** افزودن دستگاه‌های جدید تنها با اضافه کردن یک نوع جدید به `enum` انجام می‌شه و نیازی به تغییرات بزرگ در کد نیست.




## تمرین‌ها

### تمرین‌های ساده:

| شماره | تمرین                                                                                                                                                                                    | بارم |
| ----- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---- |
| 1     | مدیریت حالت‌های مختلف LED: یک `union` طراحی کن که حالت‌های مختلف LED (روشن، خاموش، چشمک‌زن) رو نگه‌داری کنه. با استفاده از `enum` نوع حالت رو تعیین کن و برنامه‌ای بنویس که LED رو کنترل کنه. | 2    |
| 2     | ذخیره داده‌های سنسورهای دما و فشار: یک `union` بساز که مقادیر دما و فشار رو توی یک فضای مشترک نگه‌داری کنه. برنامه‌ای بنویس که این مقادیر رو بخونه و نمایش بده.                             | 3    |
| 3     | مدیریت ورودی دکمه‌های دستگاه: یک `union` طراحی کن که ورودی دکمه‌های مختلف یک دستگاه رو نگه‌داری کنه و با `enum` نوع ورودی رو مشخص کن. برنامه‌ای بنویس که هر ورودی رو به درستی پردازش کنه.    | 3    |

### تمرین‌های پیشرفته:

| شماره | تمرین                                                                                                                                                                                                                                                   | بارم |
| ----- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---- |
| 4     | مدیریت پیام‌های سنسور بی‌سیم: یک `union` و `enum` طراحی کن که پیام‌های مختلف از یک سنسور بی‌سیم رو دریافت و پردازش کنه. برنامه‌ای بنویس که این پیام‌ها رو به درستی نمایش بده.                                                                                 | 4    |
| 5     | کنترل دستگاه‌های مختلف از طریق واحد کنترل مرکزی: یک واحد کنترل مرکزی داری که باید چندین دستگاه مثل فن و پمپ رو مدیریت کنه. از `union` و `enum` برای مدیریت دستورات کنترلی این دستگاه‌ها استفاده کن و برنامه‌ای بنویس که این دستورات رو به دستگاه‌ها بفرسته. | 5    |

</div>
