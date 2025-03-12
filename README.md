https://graceful-colby-ca0.notion.site/IRC-17097847c08a80b9ba30e68e199da6b8?pvs=4
# IRC

Ağ üzerinde iki yönlü iletişimi yürütmek için iki cihaza bağlantı olarak bağlanan bir ağ. Cihazlardan veri alır ve cihazlara veri gönderir. Soket adresi, IP adresi ve portun bir kombinasyonudur. TCP/IP katmanında bir soket, verilerin bir uygulamaya gönderilip gönderilmeyeceğini belirleyebilen bir port numarası olarak bağlanır. Soketteki taşıma katmanı, cihazlar arasındaki iletişimi yönetmek ve kurmak için temel mekanizmadır.

Soketler, istemci ve sunucu arasında bir iletişim cihazı veya etkileşim olarak kullanılır. İstemciden bilgi alır ve istemciye bilgi gönderir ve verileri aldıktan sonra bağlantısını keser.

---

## `*sockaddr_in Yapısı ve IPv4 Kullanımı*`

`sockaddr_in` yapısı bir IPv4 adresini ve port numarasını temsil etmek için kullanılır. Bu yapı genellikle bir soket adresini belirtmek için `bind()`, `connect()`, `sendto()` ve `recvfrom()` gibi sistem çağrılarında kullanılır.

- `AF_INET` değeri, IPv4 adreslerini belirtmek için kullanılır.
- Eğer IPv6 adresleri kullanılacaksa `AF_INET6` değeri kullanılır.

Bu ayar, soketin hangi protokol ailesini kullanacağını sistemin bilmesini sağlar.

`*Byte Sıralama (Endianness) ve htons() Fonksiyonu*`

Bilgisayar sistemlerinde iki farklı byte sıralama yöntemi vardır:

1. **Big-endian** (Büyük endian): En anlamlı byte önce gelir.
2. **Little-endian** (Küçük endian): En anlamlı byte sonra gelir.

The htons() function translates a short integer from host byte order to network byte order.

- `INADDR_ANY` kullanıldığında, soket tüm mevcut ağ arayüzlerinden (tüm IP adreslerinden) gelen bağlantıları kabul eder.
- Eğer belirli bir IP adresine bağlanmak isterseniz, `inet_addr()` veya `inet_pton()` fonksiyonlarını kullanarak IP adresini atayabilirsiniz:

```cpp
server_addr.sin_family = AF_INET;         // IPv4
server_addr.sin_port = htons(port);       // Port
server_addr.sin_addr.s_addr = INADDR_ANY; 
```

---

## **`*Soket Oluşturma (socket())*`**

- `socket()` fonksiyonu bir soket oluşturur.
- `AF_INET`: IPv4 protokol ailesini belirtir.
- `SOCK_STREAM`: TCP protokolünü kullanacağımızı belirtir.
- TCP  SOCK_STREAM, UDP ise SOCK_DGRAM kullanır.

`*TCP (SOCK_STREAM) vs UDP (SOCK_DGRAM)*`

- **TCP**: Bağlantılı, güvenilir, hata kontrolü var, veri sıralı, daha yavaş (Örn: HTTP, FTP, SSH).
- **UDP**: Bağlantısız, hızlı, güvenilir değil, paket sırası karışabilir (Örn: DNS, VoIP, online oyunlar).
- `0`: Varsayılan protokol (TCP) kullanılır.

Eğer `socket()` fonksiyonu -1 dönerse, soket oluşturma başarısız olmuştur.

- `SOCK_STREAM` için varsayılan protokol **TCP (`IPPROTO_TCP`)**'dir.
- `SOCK_DGRAM` için varsayılan protokol **UDP (`IPPROTO_UDP`)**'dir.
- `0`: Varsayılan protokolü kullanır (TCP için `IPPROTO_TCP`, UDP için `IPPROTO_UDP`).
- `IPPROTO_TCP`: TCP protokolünü kullanır.
- `IPPROTO_UDP`: UDP protokolünü kullanır.
- `IPPROTO_SCTP`: SCTP (Stream Control Transmission Protocol) protokolünü kullanır.
- `IPPROTO_RAW`: Ham soket (Raw socket) kullanır.

```cpp
sockfd = socket(AF_INET, SOCK_STREAM, 0);
int sockfd = socket(int domain, int type, int protocol);

```

---

## **`*Soket Ayarlarını Değiştirme (setsockopt())*`**

- `setsockopt()` fonksiyonu, soketin özelliklerini değiştirmek için kullanılır.
- `SO_REUSEADDR`, program sonlandıktan sonra bağlantının hemen tekrar kullanılmasını sağlar.
- Eğer `SO_REUSEADDR` **ayarlanmazsa**, aynı port hemen tekrar kullanılamaz.
- Eğer `SO_REUSEADDR` **ayarlanırsa**, aynı port başka bir program tarafından hemen kullanılabilir.
- Kodda görülen **`opt` değişkeni**, `SO_REUSEADDR` seçeneğinin etkin olup olmadığını belirleyen bir **tam sayı değişkenidir**.
- 
- **`setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))`**, belirli bir portun önceki bağlantılardan dolayı bloke olmasını engellemek için kullanılır.
- `opt` değişkeni, `1` olarak ayarlandığında bu özellik etkin olur.
- Özellikle **sunucu programlarında** ve **UDP uygulamalarında** kullanışlıdır.
- `SO_REUSEADDR`, `SO_REUSEPORT` ile karıştırılmamalıdır; ilki eski bağlantıları temizlerken, ikincisi yük dengeleme sağlar.

Başarısız olursa hata mesajı yazdırılmalıdır.

```jsx
setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0
```

```cpp
int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);

```

---

## **`*Soketi Bir IP ve Porta Bağlama (bind())*`**

- `bind()` fonksiyonu, soketi belirli bir IP adresi ve porta bağlamak için kullanılır.
- `server_addr`, `sockaddr_in` yapısından türetilmiş bir adres yapısıdır.

| `sockfd` | Bağlanacak soketin dosya tanıtıcısı (file descriptor). |
| --- | --- |

| `addr` | Bağlanacak adres bilgisini içeren yapı (`struct sockaddr`). |
| --- | --- |

| `addrlen` | `addr` yapısının boyutu (`sizeof(server_addr)`). |
| --- | --- |

`bind()` başarısız olursa, genellikle **port kullanılıyor (`EADDRINUSE`)** veya **IP adresi hatalı (`EADDRNOTAVAIL`)** olabilir.

Eğer `bind()` başarısız olursa, portun kullanımda olup olmadığı kontrol edilmelidir.

```jsx
bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0
```

---

## **`*listen() Fonksiyonu ve Bağlantı Kuyruğu*`**

- `listen()` fonksiyonu, bir **TCP sunucu soketini** **pasif moda** alarak, **bağlantı taleplerini dinlemeye başlamasını** sağlar.
- `3`: Bağlantı kuyruğundaki maksimum bekleyen istemci sayısı.

Başarısız olursa hata mesajı yazdırılmalıdır.

Eğer başarılı olursa `0` döner, başarısız olursa `-1` döner ve `errno` değişkeni ayarlanır.

**Sadece TCP (SOCK_STREAM) soketleri için geçerlidir**. UDP soketleri bağlantısız olduğu için `listen()` kullanılmaz

Gelen bağlantılar **bir kuyruğa (backlog)** alınır ve `accept()` fonksiyonu çağrılana kadar bekletilir.

- **`bind()` çağrısı olmadan `listen()` kullanılamaz**. Soket önce bir IP adresine ve porta bağlanmalıdır.
- **`accept()` çağrılmadan `listen()` tek başına yeterli değildir**. `accept()` fonksiyonu kuyruğa alınan bağlantıları işlemek için kullanılır.
- 

Bir istemci, `connect()` fonksiyonunu çağırdığında:

1. Bağlantı **kuyruğa alınır** (`listen()` çağrısında belirtilen `backlog` değerine göre).
2. **Sunucu `accept()` çağırana kadar** istemci bekler.
3. Sunucu **`accept()` çağırdığında**, en eski bağlantıyı kabul eder.
4. Eğer kuyruk **doluysa**, yeni gelen bağlantılar **reddedilebilir**.

SOMAXCONN→arastır

```jsx
listen(sockfd, 3) < 0
int listen(int sockfd, int backlog);

```

---

## **`*fd_set Yapısı ve Kullanımı*`**

`fd_set`, bir dizi dosya tanıtıcısını (file descriptor - fd) temsil eden bir veri yapısıdır. `select()` sistem çağrısında, hangi dosya tanıtıcılarının giriş, çıkış veya hata durumları için izleneceğini belirtmek için kullanılır.

Linux ve POSIX sistemlerinde, birden fazla dosya tanıtıcısını tek bir sistem çağrısı ile izlemek için `fd_set` yapısı aşağıdaki makrolarla kullanılır:

- `FD_ZERO(fd_set *set)`: `fd_set` yapısını sıfırlar, yani tüm bitleri temizler.
- `FD_SET(int fd, fd_set *set)`: Belirtilen dosya tanıtıcısını `fd_set` içine ekler.
- `FD_CLR(int fd, fd_set *set)`: Belirtilen dosya tanıtıcısını `fd_set` içinden çıkarır.
- `FD_ISSET(int fd, fd_set *set)`: Belirtilen dosya tanıtıcısının `fd_set` içinde olup olmadığını kontrol eder.

**`FD_SET(sockfd, &read_fds);`** 

Bu satır, belirtilen dosya tanıtıcısını (`sockfd`), `fd_set` içerisine ekler. Böylece, `select()` çağrısı sırasında bu soketin okunabilir olup olmadığı kontrol edilir.

- `FD_ZERO()`: `fd_set` yapısını temizler.
- `FD_SET()`: Soket dosya tanıtıcısını `fd_set` içine ekler.
- Bağlantıları izlemek için `select()` kullanılır.
    
    ```cpp
    FD_ZERO(&read_fds);        
    FD_SET(sockfd, &read_fds);
    fd_set active_fds = read_fds;
    ```
    

---

## **`*select() Fonksiyonu ve Kullanımı*`**

`select()` fonksiyonu, birden fazla dosya tanıtıcısını (file descriptor - fd) izlemek için kullanılan bir sistem çağrısıdır. Birden fazla giriş/çıkış kaynağını aynı anda dinleyerek, hangisinin hazır olduğunu belirleyebilir.

- `select()`, belirli soketlerde aktivite olup olmadığını kontrol eder.
- `max_fd + 1`: En büyük dosya tanıtıcısını belirtir.
- `NULL`: Yazma ve hata kontrol listeleri boş bırakılmıştır.
- `NULL`: Zaman aşımı beklenmez, sonsuza kadar bekler.

Bağlantı varsa, `activity` 0'dan farklı bir değer alır.

`*Parametreler*`

- `nfds`: İzlenecek en büyük dosya tanıtıcısının değeri + 1 olarak ayarlanmalıdır.
- `readfds`: Okuma işlemi için izlenecek dosya tanıtıcılarını içeren `fd_set` yapısı.
- `writefds`: Yazma işlemi için izlenecek dosya tanıtıcılarını içeren `fd_set` yapısı.
- `exceptfds`: Hata durumlarını izlemek için kullanılan `fd_set` yapısı.
- `timeout`: `select()` fonksiyonunun bekleme süresini belirler. `NULL` verilirse süresiz bekler.

Fonksiyon, izlenen dosya tanıtıcılarından en az biri belirtilen işlem için hazır olduğunda döner.

`*Dönüş Değerleri*`

- **Pozitif sayı**: Hazır dosya tanıtıcılarının sayısını döndürür.
- **0**: Hiçbir dosya tanıtıcısı belirtilen süre içinde hazır olmadı.
- **1**: Hata oluştu, `errno` değişkenine hata kodu atanır.

`*select() Kullanımına Alternatifler*`

1. **`poll()`**: Daha fazla özellik sunar ancak aynı temel mantığa sahiptir.
2. **`epoll()`**: Linux sistemlerinde daha verimli bir alternatif olarak kullanılır.

```jsx
int activity = select(max_fd + 1, &active_fds, NULL, NULL, NULL);
int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
```

---

- `FD_ISSET()`, belirli bir soketin aktif olup olmadığını kontrol eder.

```jsx
FD_ISSET(sockfd, &active_fds)
```

- `FD_SET()`, yeni istemci soketini `fd_set` içine ekler.

Bağlantı kabul edildikten sonra istemci ile veri alışverişi yapılabilir.

```jsx
FD_SET(client_sockfd, &read_fds);
```

---

## **`*accept() Fonksiyonu ve Kullanımı*`**

- `accept()`, gelen bir istemci bağlantısını kabul eder ve yeni bir soket döndürür.

| `sockfd` | Sunucu tarafında oluşturulmuş ve `listen()` ile dinleme moduna geçirilmiş soketin dosya tanıtıcısı. |
| --- | --- |
| `addr` | Bağlantı kurulan istemcinin adresini tutacak `struct sockaddr` türünde bir gösterici. |
| `addrlen` | `addr` yapısının boyutunu belirten bir değişkenin adresi. |

Eğer istemcinin adres bilgisine ihtiyacınız yoksa, `addr` ve `addrlen` parametreleri `NULL` olarak verilebilir.

```jsx
accept(sockfd, NULL, NULL)
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

```