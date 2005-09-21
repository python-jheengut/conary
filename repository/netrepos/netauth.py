#
# Copyright (c) 2004-2005 rPath, Inc.
#
# This program is distributed under the terms of the Common Public License,
# version 1.0. A copy of this license should have been distributed with this
# source file in a file called LICENSE. If it is not present, the license
# is always available at http://www.opensource.org/licenses/cpl.php.
#
# This program is distributed in the hope that it will be useful, but
# without any waranty; without even the implied warranty of merchantability
# or fitness for a particular purpose. See the Common Public License for
# full details.
#
import md5
import os
import re
import sqlite3
import sys

from repository.netclient import UserAlreadyExists, GroupAlreadyExists, PermissionAlreadyExists, UserNotFound

class NetworkAuthorization:
    def check(self, authToken, write = False, admin = False, label = None, trove = None):
        if label and label.getHost() != self.name:
            raise RepositoryMismatch

        if not authToken[0]:
            return False

        stmt = """
            SELECT item, salt, password FROM
               (SELECT * FROM Users WHERE user=?) AS Users
            INNER JOIN UserGroupMembers ON
                UserGroupMembers.userId = Users.userId
            INNER JOIN Permissions ON 
                UserGroupMembers.userGroupId = Permissions.userGroupId
            LEFT OUTER JOIN Items ON 
                Permissions.itemId = Items.itemId
        """

        params = [ authToken[0] ]

        where = []
        if label:
            where.append(" labelId=(SELECT labelId FROM Labels WHERE " \
                            "label=?) OR labelId=0")
            params.append(label.asString())

        if write:
            where.append("write=1")

        if admin:
            where.append("admin=1")

        if where:
            stmt += "WHERE " + " AND ".join(where)

        cu = self.db.cursor()
        cu.execute(stmt, params)

        for (troveName, salt, password) in cu:
            if troveName=='ALL' or not trove:
                regExp = None
            else:
                regExp = self.reCache.get(troveName, None)
                if regExp is None:
                    regExp = re.compile(troveName)
                    self.reCache[troveName] = regExp

            if not regExp or regExp.match(trove):
                m = md5.new()
                m.update(salt)
                m.update(authToken[1])
                if m.hexdigest() == password:
                    return True

        return False
        
    def checkTrove(self, pattern, trove):
        if pattern=='ALL':
            return True

        regExp = self.reCache.get(pattern, None)
        if regExp is None:
            regExp = re.compile(pattern)
            self.reCache[trove] = regExp

        if regExp.match(trove):
            return True

        return False

    def checkPassword(self, salt, password, challenge):
        m = md5.new()
        m.update(salt)
        m.update(challenge)

        return m.hexdigest() == password

    def checkUserPass(self, authToken, label = None):
        if label and label.getHost() != self.name:
            raise RepositoryMismatch

        cu = self.db.cursor()

        stmt = "SELECT salt, password FROM Users WHERE user=?"
        cu.execute(stmt, authToken[0])

        for (salt, password) in cu:
            m = md5.new()
            m.update(salt)
            m.update(authToken[1])
            if m.hexdigest() == password:
                return True

        return False

    def checkIsFullAdmin(self, user, password):
        cu = self.db.cursor()
        cu.execute("""SELECT salt, password  
                        FROM userPermissions 
                        WHERE User=? AND admin=1""", user)

        for (salt, cryptPassword) in cu:
            if not self.checkPassword(salt, cryptPassword, password):
                return False
            return True

        return False

    def addAcl(self, userGroup, trovePattern, label, write, capped, admin):
        cu = self.db.cursor()

        # XXX This functionality is available in the TroveStore class
        #     refactor so that the code is not in two places
        if trovePattern:
            cu.execute("SELECT * FROM Items WHERE item=?", trovePattern)
            itemId = cu.fetchone()
            if itemId:
                itemId = itemId[0]
            else:
                cu.execute("INSERT INTO Items VALUES(NULL, ?)", trovePattern)
                itemId = cu.lastrowid
        else:
            itemId = 0

        if label:
            cu.execute("SELECT * FROM Labels WHERE label=?", label)
            labelId = cu.fetchone()
            if labelId:
                labelId = labelId[0]
            else:
                cu.execute("INSERT INTO Labels VALUES(NULL, ?)", label)
                labelId = cu.lastrowid
        else:
            labelId = 0


        try:
            cu.execute("""INSERT INTO Permissions
                            SELECT userGroupId, ?, ?, ?, ?, ? FROM
                                (SELECT userGroupId FROM userGroups WHERE
                                userGroup=?)
                        """, labelId, itemId, write, capped, admin, userGroup)
        except sqlite3.ProgrammingError, e:
            if str(e) == 'columns userGroupId, labelId, itemId are not unique':
                self.db.rollback()
                raise PermissionAlreadyExists, "labelId: '%s', itemId: '%s'" % (labelId, itemId)
            raise

        self.db.commit()

    def editAcl(self, userGroup, oldTroveId, oldLabelId, troveId, labelId,
            write, capped, admin):
  
        cu = self.db.cursor()

        userGroupId = self.getGroupIdByName(userGroup)

        try:
            cu.execute("""UPDATE Permissions SET 
                              labelId = ?,
                              itemId = ?,
                              write = ?,
                              capped = ?,
                              admin = ?
                            WHERE userGroupId=? AND
                                labelId=? AND itemId=?
                        """, labelId, troveId, write, capped, admin, 
                        userGroupId, oldLabelId, oldTroveId)
        except sqlite3.ProgrammingError, e:
            if str(e) == 'columns userGroupId, labelId, itemId are not unique':
                self.db.rollback()
                raise PermissionAlreadyExists, "labelId: '%s', itemId: '%s'" % (labelId, itemId)
            raise

        self.db.commit()

    def deleteAcl(self, userGroupId, labelId, itemId):
        cu = self.db.cursor()
        
        stmt = """DELETE FROM Permissions
                  WHERE userGroupId=? AND
                        (labelId=? OR (labelId IS NULL AND ? IS NULL)) AND
                        (itemId=? OR (itemId IS NULL AND ? IS NULL))"""

        cu.execute(stmt, userGroupId, labelId, labelId, itemId, itemId)
        self.db.commit()

    def _uniqueUser(self, cu, user):
        """
        Returns True if the username is unique.  Raises UserAlreadyExists
        if it is not unique
        """
        cu.execute("""
            SELECT COUNT(userId)
            FROM Users WHERE LOWER(User)=LOWER(?)
        """, user)
        if cu.next()[0]:
            raise UserAlreadyExists, 'user: %s' % user
        return True

    def _uniqueUserGroup(self, cu, usergroup):
        """
        Returns True if the username is unique.  Raises UserAlreadyExists
        if it is not unique
        """
        cu.execute("""
            SELECT COUNT(userGroupId)
            FROM UserGroups WHERE LOWER(UserGroup)=LOWER(?)
        """, usergroup)
        if cu.next()[0]:
            raise GroupAlreadyExists, 'usergroup: %s' % usergroup
        return True

    def addUser(self, user, password):
        salt = os.urandom(4)
        
        m = md5.new()
        m.update(salt)
        m.update(password)
        return self.addUserByMD5(user, salt, m.hexdigest())

    def addUserByMD5(self, user, salt, password):

        #Insert the UserGroup first, but since usergroups can be added 
        #and deleted at will, and sqlite uses a MAX(id)+1 approach to 
        #sequencing, use max(userId, userGroupId)+1 so that userId and
        #userGroupId can be in sync.  This will leave lots of holes, and
        #will probably need to be changed if conary moves to another db.
        cu = self.db.cursor()

        #Check to make sure the user is unique in both the user and UserGroup
        #tables
        self._uniqueUserGroup(cu, user)
        self._uniqueUser(cu, user)

        try:
            cu.execute("""INSERT INTO UserGroups 
    SELECT MAX(
        (SELECT IFNULL(MAX(userId),0) FROM Users),
        (SELECT IFNULL(MAX(userGroupId),0) FROM UserGroups)
    )+1, ? """, user)
        except sqlite3.ProgrammingError, e:
            if str(e) == 'column userGroup is not unique':
                raise GroupAlreadyExists, 'group: %s' % user
            raise

        userGroupId = cu.lastrowid

        try:
            cu.execute("INSERT INTO Users VALUES (?, ?, ?, ?)",
                       (userGroupId, user, salt, password))
        except sqlite3.ProgrammingError, e:
            if str(e) == 'column user is not unique':
                raise UserAlreadyExists, 'user: %s' % user
            raise

        userId = cu.lastrowid
        cu.execute("INSERT INTO UserGroupMembers VALUES (?, ?)", 
                   userGroupId, userGroupId)

        self.db.commit()
        return userId

    def deleteUserByName(self, user, commit = True):
        cu = self.db.cursor()

        sql = "SELECT userId FROM Users WHERE user=?"
        cu.execute(sql, user)
        try:
            userId = cu.next()[0]
        except StopIteration:
            raise UserNotFound(user)
        
        return self.deleteUser(userId, user, commit)

    def deleteUserById(self, userId, commit = True):
        cu = self.db.cursor()

        sql = "SELECT user FROM Users WHERE userId=?"
        cu.execute(sql, userId)
        try:
            user = cu.next()[0]
        except StopIteration:
            raise UserNotFound(user)
        
        return self.deleteUser(userId, user, commit)

    def deleteUser(self, userId, user, commit = True):
        # Need to do a lot of stuff:
        # UserGroups, Users, and all ACLs

        cu = self.db.cursor()

        try:
            #First delete the user from all the groups
            sql = "DELETE from UserGroupMembers WHERE userId=?"
            cu.execute(sql, userId)

                
            #Then delete the UserGroup created with the name of that user
            try:
                self.deleteGroup(user, False)
            except StopIteration, e:
                # Ignore the StopIteration error as it means there was no
                # Group matching that name.  Probably because the group
                # was deleted beforehand
                pass

            #Now delete the user-self
            sql = "DELETE from Users WHERE userId=?"
            cu.execute(sql, userId)

            if commit: 
                self.db.commit()
        except Exception, e:
            if commit:
                self.db.rollback()
            
            raise e
        return True

    def changePassword(self, user, newPassword):
        cu = self.db.cursor()

        salt = os.urandom(4)
        
        m = md5.new()
        m.update(salt)
        m.update(newPassword)
        password = m.hexdigest()

        cu.execute("UPDATE Users SET password=?, salt=? WHERE user=?", 
                   password, salt, user)
        self.db.commit()

    def getUserGroups(self, user):
        cu = self.db.cursor()
        cu.execute("""SELECT UserGroups.userGroup
                      FROM UserGroups, Users, UserGroupMembers 
                      WHERE UserGroups.userGroupId = UserGroupMembers.userGroupId AND
                            UserGroupMembers.userId = Users.userId AND
                            Users.user = ?""", user)
        
        return [row[0] for row in cu]

    def iterUsers(self):
        cu = self.db.cursor()
        cu.execute("SELECT userId, user FROM Users")
        
        for row in cu:
            yield row

    def iterGroups(self):
        cu = self.db.cursor()
        cu.execute("SELECT userGroupId, userGroup FROM UserGroups")
        
        for row in cu:
            yield row

    def iterGroupsByUserId(self, userId):
        cu = self.db.cursor()
        cu.execute("""SELECT UserGroups.userGroupId, UserGroups.userGroup
                      FROM UserGroups INNER JOIN UserGroupMembers ON
                      UserGroups.userGroupId = UserGroupMembers.userGroupId
                      WHERE UserGroupMembers.userId=?""", userId)

        for row in cu:
            yield row

    def iterGroupMembers(self, userGroupId):
        cu = self.db.cursor()
        cu.execute("""SELECT Users.user FROM UserGroupMembers, Users
                      WHERE Users.userId = UserGroupMembers.userId AND
                      UserGroupMembers.userGroupId=?""", userGroupId)

        for row in cu:
            yield row[0]

    def iterPermsByGroupId(self, userGroupId):
        cu = self.db.cursor()
        cu.execute("""SELECT Permissions.labelId, Labels.label,
                             PerItems.itemId, PerItems.item,
                             write, capped, admin
                      FROM Permissions
                      LEFT OUTER JOIN Items AS PerItems ON
                          PerItems.itemId = Permissions.itemId
                      LEFT OUTER JOIN Labels ON
                          Permissions.labelId = Labels.labelId
                      WHERE userGroupId=?""", userGroupId)

        for row in cu:
            yield row

    def getGroupIdByName(self, userGroupName):
        cu = self.db.cursor()

        cu.execute("SELECT userGroupId FROM UserGroups WHERE userGroup=?",
            userGroupName)

        return cu.next()[0]

    def getUserIdByName(self, userName):
        cu = self.db.cursor()

        cu.execute("SELECT userId FROM Users WHERE user=?",
                   userName)

        return cu.next()[0]

    def addGroup(self, userGroupName):
        cu = self.db.cursor()

        #Check to make sure the group is unique
        self._uniqueUserGroup(cu, userGroupName)

        try:
            cu.execute("INSERT INTO UserGroups (userGroup) VALUES (?)", userGroupName)
        except sqlite3.ProgrammingError, e:
            self.db.rollback()
            if str(e) == 'column userGroup is not unique':
                raise GroupAlreadyExists, "group: %s" % userGroupName
            raise

        self.db.commit()
        return cu.lastrowid

    def renameGroup(self, userGroupId, userGroupName):
        cu = self.db.cursor()

        #Check to make sure the group is unique
        self._uniqueUserGroup(cu, userGroupName)

        try:
            cu.execute("UPDATE UserGroups SET userGroup=? WHERE userGroupId=?", userGroupName, userGroupId)
        except sqlite3.ProgrammingError, e:
            self.db.rollback()
            if str(e) == 'column userGroup is not unique':
                raise GroupAlreadyExists, "group: %s" % userGroupName
            raise

        self.db.commit()

    def updateGroupMembers(self, userGroupId, members):
        #Do this in a transaction
        cu = self.db.cursor()
        
        #First drop all the current members
        cu.execute ("DELETE FROM UserGroupMembers WHERE userGroupId=?", userGroupId)

        #now add the new members
        for userId in members:
            self.addGroupMember(userGroupId, userId, False)

        self.db.commit()

    def addGroupMember(self, userGroupId, userId, commit = True):
        cu = self.db.cursor()

        cu.execute("INSERT INTO UserGroupMembers VALUES(?, ?)",
                   userGroupId, userId)
        if commit:
            self.db.commit()

    def deleteGroup(self, userGroupName, commit = True):
        return self.deleteGroupById(self.getGroupIdByName(userGroupName), commit)

    def deleteGroupById(self, userGroupId, commit = True):
        cu = self.db.cursor()
        cu.execute("DELETE FROM Permissions WHERE userGroupId=?", userGroupId)
        cu.execute("DELETE FROM UserGroupMembers WHERE userGroupId=?", userGroupId)
        cu.execute("DELETE FROM UserGroups WHERE userGroupId=?", userGroupId)

        #Note, there could be a user left behind with no associated group 
        #if the group being deleted was created with a user.  This user is not
        #deleted because it is possible for this user to be a member of
        #another group.
        if commit:
            self.db.commit()

    def iterItems(self):
        cu = self.db.cursor()

        cu.execute("SELECT itemId, item FROM Items")
        for row in cu:
            yield row

    def iterLabels(self):
        cu = self.db.cursor()
       
        cu.execute("SELECT labelId, label FROM Labels")
        for row in cu:
            yield row

    def __init__(self, db, name):
        self.name = name
        self.db = db
        self.reCache = {}

        cu = self.db.cursor()
        cu.execute("SELECT tbl_name FROM sqlite_master WHERE type "
                   "in ('table', 'view')")
        tables = [ x[0] for x in cu ]

        commit = False
        
        if "Users" not in tables:
            cu.execute("""CREATE TABLE Users (userId INTEGER PRIMARY KEY,
                                              user STRING UNIQUE,
                                              salt BINARY,
                                              password STRING)""")
            commit = True

        if "UserGroups" not in tables:
            cu.execute("""CREATE TABLE UserGroups (
                                           userGroupId INTEGER PRIMARY KEY,
                                           userGroup STRING UNIQUE)""")
            commit = True

        if "UserGroupMembers" not in tables:
            cu.execute("""CREATE TABLE UserGroupMembers (
                                            userGroupId INTEGER,
                                            userId INTEGER)""")
            cu.execute("""CREATE INDEX UserGroupMembersIdx ON
                                            UserGroupMembers(userGroupId)""")
            cu.execute("""CREATE INDEX UserGroupMembersIdx2 ON
                                            UserGroupMembers(userId)""")
            commit = True

        if "Permissions" not in tables:
            cu.execute("""CREATE TABLE Permissions (userGroupId INTEGER,
                                                    labelId INTEGER NOT NULL,
                                                    itemId INTEGER NOT NULL,
                                                    write INTEGER,
                                                    capped INTEGER,
                                                    admin INTEGER)""")
            cu.execute("""CREATE UNIQUE INDEX PermissionsIdx
                          ON Permissions(userGroupId, labelId, itemId)""")

            if "Items" in tables:
                cu.execute("""INSERT INTO Items VALUES(0, 'ALL')""")
            if "Labels" in tables:
                cu.execute("""INSERT INTO Labels VALUES (0, 'ALL')""")

            commit = True

        if "UserPermissions" not in tables:
            cu.execute("""CREATE VIEW UserPermissions AS
                  SELECT Users.user AS user,
                         Users.salt AS salt,
                         Users.password as password,
                         PerItems.item AS permittedTrove,
                         Permissions.labelId AS permittedLabelId,
                         Labels.label AS permittedLabel,
                         Permissions.admin AS admin,
                         Permissions.write AS write,
                         Permissions._ROWID_ as aclId
                      FROM Users JOIN UserGroupMembers ON
                          Users.userId = UserGroupMembers.userId
                      INNER JOIN Permissions ON
                          UserGroupMembers.userGroupId = Permissions.userGroupId
                      LEFT OUTER JOIN Items AS PerItems ON
                          PerItems.itemId = Permissions.itemId
                      LEFT OUTER JOIN Labels ON
                          Permissions.labelId = Labels.labelId
            """)
            commit = True

        if commit:
            self.db.commit()

class RepositoryMismatch(Exception):
    pass

class InsufficientPermission(Exception):
    pass
