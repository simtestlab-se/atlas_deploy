# AWS EC2 Deployment Setup Guide

This guide explains how to set up GitHub Actions for deploying your MkDocs site to AWS EC2.

## Prerequisites

1. **AWS EC2 Instance**: A running Ubuntu EC2 instance
2. **Domain Name**: (Optional) A domain pointing to your EC2 instance
3. **SSH Access**: SSH key pair for accessing your EC2 instance
4. **GitHub Repository**: This repository with proper permissions

## Step 1: Prepare Your EC2 Instance

### 1.1 Launch EC2 Instance
- Launch an Ubuntu 20.04 or 22.04 EC2 instance
- Configure security group to allow:
  - SSH (port 22) from your IP
  - HTTP (port 80) from anywhere (0.0.0.0/0)
  - HTTPS (port 443) from anywhere (0.0.0.0/0)

### 1.2 Run Setup Script
Copy and run the setup script on your EC2 instance:

```bash
# SSH into your EC2 instance
ssh -i your-key.pem ubuntu@your-ec2-public-ip

# Download and run the setup script
wget https://raw.githubusercontent.com/your-username/atlas/main/scripts/setup-ec2.sh
chmod +x setup-ec2.sh
sudo ./setup-ec2.sh
```

## Step 2: Configure GitHub Secrets

Go to your GitHub repository → Settings → Secrets and variables → Actions

Add the following secrets:

### Required Secrets

| Secret Name | Description | Example Value |
|-------------|-------------|---------------|
| `EC2_HOST` | Your EC2 instance public IP or domain | `3.15.24.125` or `docs.simtestlab.com` |
| `EC2_USERNAME` | SSH username for your EC2 instance | `ubuntu` (for Ubuntu AMI) |
| `EC2_SSH_KEY` | Private SSH key content | Content of your `.pem` file |
| `DEPLOY_PATH` | Path where files will be deployed | `/var/www/simtestlab` |

### How to get SSH Key content:
```bash
# On your local machine, display your private key
cat your-ec2-key.pem
```
Copy the entire content including `-----BEGIN RSA PRIVATE KEY-----` and `-----END RSA PRIVATE KEY-----`

## Step 3: Set Up SSH Key Authentication

### 3.1 Add GitHub Actions Public Key
The workflow will use the private key you provided to connect. Make sure the corresponding public key is in your EC2 instance's `~/.ssh/authorized_keys` file.

If you're using the same key pair you used to launch the EC2 instance, this should already be configured.

### 3.2 Test SSH Connection
Test the connection from your local machine:
```bash
ssh -i your-key.pem ubuntu@your-ec2-ip "echo 'SSH connection successful!'"
```

## Step 4: Configure Nginx (Done by setup script)

The setup script creates an Nginx configuration at `/etc/nginx/sites-available/simtestlab` with:
- Static file serving optimized for MkDocs
- Gzip compression
- Security headers
- Caching for static assets

## Step 5: Domain Configuration (Optional)

### 5.1 DNS Setup
Point your domain to your EC2 instance:
- Create an A record pointing to your EC2 public IP
- Create a CNAME record for `www` pointing to your main domain

### 5.2 Update Nginx Configuration
Edit the server configuration:
```bash
sudo nano /etc/nginx/sites-available/simtestlab
```
Update the `server_name` directive with your actual domain:
```nginx
server_name yourdomain.com www.yourdomain.com;
```

### 5.3 Restart Nginx
```bash
sudo nginx -t
sudo systemctl reload nginx
```

## Step 6: SSL Certificate (Recommended)

Set up Let's Encrypt SSL certificate:
```bash
sudo apt install certbot python3-certbot-nginx
sudo certbot --nginx -d yourdomain.com -d www.yourdomain.com
```

## Step 7: Test Deployment

1. Push changes to your repository
2. Check the Actions tab for deployment status
3. Visit your EC2 IP or domain to see your deployed site

## Workflow Triggers

The GitHub Actions workflow triggers on:
- Push to `main`, `master`, or `mkdocs-enabled-submodules-added` branches
- Pull requests (build test only)
- Manual trigger via GitHub Actions UI

## Troubleshooting

### Common Issues:

1. **SSH Connection Failed**
   - Check EC2 security group allows SSH from GitHub Actions IPs
   - Verify SSH key format in GitHub secrets
   - Ensure EC2 instance is running

2. **Permission Denied on Deployment**
   - Check that the user owns the deployment directory
   - Verify file permissions: `sudo chown -R ubuntu:ubuntu /var/www/simtestlab`

3. **Nginx 403 Forbidden**
   - Check file permissions: `sudo chmod -R 755 /var/www/simtestlab`
   - Verify index.html exists in the deployment directory

4. **Build Failures**
   - Check MkDocs configuration in `mkdocs.yml`
   - Verify all required plugins are in `requirements.txt`

### Useful Commands:

```bash
# Check Nginx status
sudo systemctl status nginx

# View Nginx error logs
sudo tail -f /var/log/nginx/error.log

# Check deployment directory
ls -la /var/www/simtestlab/

# Test Nginx configuration
sudo nginx -t

# Reload Nginx without downtime
sudo systemctl reload nginx
```

## Security Considerations

1. **SSH Key Management**: Store SSH keys securely in GitHub secrets
2. **Firewall**: Only allow necessary ports (22, 80, 443)
3. **Fail2ban**: Installed by setup script to prevent brute force attacks
4. **SSL**: Use Let's Encrypt for HTTPS encryption
5. **Updates**: Regularly update your EC2 instance packages

## Monitoring and Maintenance

1. **Log Monitoring**: Regularly check Nginx access and error logs
2. **Updates**: Keep your EC2 instance and packages updated
3. **Backups**: Consider backing up your deployment configuration
4. **Performance**: Monitor site performance and server resources

## Cost Optimization

1. **Instance Type**: Use appropriate instance size (t3.micro for small sites)
2. **Reserved Instances**: Consider reserved instances for production
3. **Monitoring**: Set up CloudWatch billing alerts
4. **Auto-scaling**: Consider auto-scaling for high-traffic sites