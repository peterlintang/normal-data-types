# first run cmd: curl https://api.github.com/users/peterlintang/repos?per_page=100 > repos.json
import json
import subprocess

# 加载仓库列表
with open('repos.json', 'r', encoding='utf-8') as file:
    repos = json.load(file)

# 遍历每个仓库
for repo in repos:
    # 获取仓库的克隆URL
    clone_url = repo['clone_url']
    # 执行克隆操作
    print(f"Cloning {clone_url}...")
    subprocess.run(['git', 'clone', clone_url])
