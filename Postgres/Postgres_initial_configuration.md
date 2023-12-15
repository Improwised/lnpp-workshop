# How to install PostgreSQL in local

### Installation

step 1: Install postgresql in local

```
sudo pacman -Syu postgresql
```

step 2: switch to postgres user

```
sudo -i -u postgres
```

step 3: Initial Configuration

```
initdb -D /var/lib/postgres/data
```

step 4 : start postgresql services

```
systemctl start postgresql.service
```

step 5: create superuser

```
createuser -S devarshi.trivedi
```

step 6: create database

```
createdb watch_time
```

step 7: go to watch_time

**************************************************psql is postgresql client**************************************************

```
psql -d myDatabaseName
```
<br/>

### Extras

what does sudo -i mean ?

```
In a simple manner, the -i option in the sudo command is like saying "pretend to be someone else and start fresh." When you use sudo -i, you're asking to simulate an initial login as a different user.

For example, if you run:

sudo -i -u postgres
It means: "Become the 'postgres' user as if you just logged in, giving you a clean slate and setting up the environment as if you were that user from the beginning."

This can be handy when you need to perform actions with the permissions and environment of another user, in this case, "postgres".
```

### Links

Arch Linux →  [https://wiki.archlinux.org/title/PostgreSQL](https://wiki.archlinux.org/title/PostgreSQL)

User creation → [https://phoenixnap.com/kb/postgres-create-user](https://phoenixnap.com/kb/postgres-create-user)
